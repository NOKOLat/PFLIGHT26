#include "StateManager/state_manager.hpp"
#include "StateFactory/state_factory.hpp"
#include "isr_manager.hpp"


// コンストラクタ
StateManager::StateManager(StateID init_state_id, uint32_t loop_time_us){

    this->init_state_id_ = init_state_id;
    state_context_.loop_time_us = loop_time_us;
}

StateContext& StateManager::getContext() {

    return state_context_;
}

void StateManager::changeState(StateID state_id) {

    // Factoryを使用して新しい状態を生成
    std::unique_ptr<StateInterface> new_state = StateFactory::createState(state_id);

    // ポインタの安全性を確認
    if (!new_state) {

        printf("[StateManager::changeState] Failed to create state (ID: %s)\n", stateIdToString(state_id));
        return;
    }

    // 新しい状態に遷移
    current_state_ = std::move(new_state);

    // 次の状態を出力
    printf("\n==================================\n");
    printf("[StateManager] Change State To %s\n", stateIdToString(state_id));

}

void StateManager::update() {

    // 初回実行時は初期化を行う
    if (is_first_execution_) {

        init();
        return;
    }

    // SBUSデータの更新・フェイルセーフ判定
    updateSBUS();

    // 無線通信データの更新

    // あとでLidarからのデータを受け取る部分を実装する
    //
    //

    // 現在の状態が有効でない場合
    if (!current_state_) {

        printf("[StateManager::update] StateInstance Error\n");
        changeState(StateID::EMERGENCY_STATE);
    }

	// 現在の状態の更新処理
	StateResult result = current_state_->update(state_context_);

	// 処理に失敗した場合
	if (result.success == ProcessStatus::FAILURE){

		printf("[StateManager::update] State Update Failed\n");
		changeState(StateID::EMERGENCY_STATE);
		return;
	}

	// 状態遷移が必要な場合
	if (result.should_transition == TransitionFlag::SHOULD_TRANSITION) {

		// 状態遷移を実行（Factoryの呼び出しはchangeState内で行われる）
		changeState(result.next_state_id);
	}
}

void StateManager::updateSBUS() {

    if (!state_context_.instances.sbus_receiver.has_value()) {
        return;
    }

    nokolat::SBUS& sbus = state_context_.instances.sbus_receiver.value();
    const nokolat::SBUS_DATA& sbus_data = sbus.getData();

    state_context_.control_input.data = sbus_data.data;
    state_context_.control_input.failsafe = sbus_data.failsafe;
    state_context_.control_input.framelost = sbus_data.framelost;

    // SBUSデータをリスケーリング
    state_context_.rescaled_sbus_data = nokolat::SBUSRescaler::rescale(sbus_data.data);

    // フェイルセーフ判定 (SBUSフレーム内のフラグ)
    if (state_context_.control_input.failsafe) {

        printf("[StateManager::updateSBUS] SBUS FailSafe\n");
        changeState(StateID::EMERGENCY_STATE);
    }

    // タイムアウトフェイルセーフ判定 (一定時間SBUSの更新がない場合)
    constexpr uint32_t SBUS_TIMEOUT_MS = 500;
    if (HAL_GetTick() - ISRManager::getLastValidFrameTick() > SBUS_TIMEOUT_MS) {

        printf("[StateManager::updateSBUS] SBUS Timeout FailSafe (%lu ms)\n",
               HAL_GetTick() - ISRManager::getLastValidFrameTick());
        changeState(StateID::EMERGENCY_STATE);
    }
}

// 初期化処理
// インスタンスの初期化、静的な設定を行う
// 通信などの失敗する可能性があるチェックはInitStateで行う
void StateManager::init() {

    // 初回実行時のみ実行
    if (!is_first_execution_) {

        return;
    }

    printf("[StateManager] InitStart\n");

    // 1. 初回実行フラグをクリア
    is_first_execution_ = false;

    // 2. 使用するインスタンスの初期化

    // 2-0 センサーマネージャーの初期化
    state_context_.instances.sensor_manager.emplace(state_context_.pin_config.sensor_i2c);

    // 2-1 PWM制御ユーティリティの初期化（モーター・サーボはPwmManager内で管理）
    state_context_.instances.pwm_controller.emplace();

    // 2-2 姿勢推定EKFの初期化(6軸モード: IMUのみ使用、magnetometerなし、50Hz)
    state_context_.instances.attitude_ekf.emplace();
    AttitudeEKF_Init(&state_context_.instances.attitude_ekf.value(), SS_DT);

    // 2-2-2 高度推定の初期化
    state_context_.instances.altitude_estimator.emplace();
    state_context_.instances.altitude_estimator->Init();

    // 2-3角度制御用PID(kp, ki, kd, dt [秒])
    state_context_.instances.angle_roll_pid.emplace(state_context_.pid_gains.angle_kp, state_context_.pid_gains.angle_ki, state_context_.pid_gains.angle_kd,  state_context_.loop_time_us / 1000000.0f);
    state_context_.instances.angle_pitch_pid.emplace(state_context_.pid_gains.angle_kp, state_context_.pid_gains.angle_ki, state_context_.pid_gains.angle_kd, state_context_.loop_time_us / 1000000.0f);
    state_context_.instances.angle_yaw_pid.emplace(state_context_.pid_gains.angle_kp, state_context_.pid_gains.angle_ki, state_context_.pid_gains.angle_kd, state_context_.loop_time_us / 1000000.0f);

    // 2-3-2角速度制御用PID(kp, ki, kd, dt [秒])
    state_context_.instances.rate_roll_pid.emplace(state_context_.pid_gains.rate_kp, state_context_.pid_gains.rate_ki, state_context_.pid_gains.rate_kd, state_context_.loop_time_us / 1000000.0f);
    state_context_.instances.rate_pitch_pid.emplace(state_context_.pid_gains.rate_kp, state_context_.pid_gains.rate_ki, state_context_.pid_gains.rate_kd, state_context_.loop_time_us / 1000000.0f);
    state_context_.instances.rate_yaw_pid.emplace(state_context_.pid_gains.rate_kp, state_context_.pid_gains.rate_ki, state_context_.pid_gains.rate_kd, state_context_.loop_time_us / 1000000.0f);

    // 2-4 SBUS
    state_context_.instances.sbus_receiver.emplace();

    // ISRマネージャにSBUSインスタンスとUARTハンドルを登録
    if (state_context_.instances.sbus_receiver.has_value()) {

    	//printf("[Debug] Using SBUS PORT: UART2\n");
        //ISRManager::registerSBUS(&state_context_.instances.sbus_receiver.value(), state_context_.pin_config.debug_uart);
        ISRManager::registerSBUS(&state_context_.instances.sbus_receiver.value(), state_context_.pin_config.sbus_uart);
    }

    // 3. 初期状態を生成
    current_state_ = StateFactory::createState(init_state_id_);


    printf("[StateManager] All Instances Generated\n");
}
