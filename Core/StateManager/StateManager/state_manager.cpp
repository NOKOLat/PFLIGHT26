#include "StateManager/state_manager.hpp"
#include "StateFactory/state_factory.hpp"
#include "isr_manager.hpp"
#include "../../Config/sensor_config.hpp"


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

    // SBUSデータをリスケーリング
    state_context_.rescaled_sbus_data = nokolat::SBUSRescaler::rescale(sbus_data.data);

    // SBUSプロトコルフラグをセット
    state_context_.rescaled_sbus_data.failsafe = sbus_data.failsafe;
    state_context_.rescaled_sbus_data.framelost = sbus_data.framelost;

    // フェイルセーフ判定 (SBUSフレーム内のフラグ)
    if (state_context_.rescaled_sbus_data.failsafe) {

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

    // 2-1 センサー設定を準備（StateManager がファサード層として集約）
    SensorConfig sensor_config = {
        .i2c_handle = state_context_.pin_config.sensor_i2c,
        .i2c_addresses = {
            .icm42688p_addr = 0x69,
            .bmm350_addr = 0x14,  // BMM350（磁気） デフォルトアドレス
            .dps368_addr = 0x77   // DPS368（気圧・温度） デフォルトアドレス
        },
        .enable_imu = true,
        .enable_magnetometer = true,
        .enable_barometer = true,
        .imu_calib = {
            .enable_auto_calibration = CalibrationConfig::ENABLE_CALIBRATION,
            .manual_accel_offset = {
                CalibrationConfig::MANUAL_ACCEL_OFFSET_X,
                CalibrationConfig::MANUAL_ACCEL_OFFSET_Y,
                CalibrationConfig::MANUAL_ACCEL_OFFSET_Z
            },
            .manual_gyro_offset = {
                CalibrationConfig::MANUAL_GYRO_OFFSET_X,
                CalibrationConfig::MANUAL_GYRO_OFFSET_Y,
                CalibrationConfig::MANUAL_GYRO_OFFSET_Z
            }
        },
        .altitude_calib = {
            .sample_count = 10,
            .reference_gravity = 9.80665f
        }
    };

    // 2-2 姿勢推定の初期化（SensorConfigを渡す）
    state_context_.attitude_estimation.initialize(sensor_config);

    // 2-3 PWM制御ユーティリティの初期化（モーター・サーボはPwmManager内で管理）
    state_context_.instances.pwm_controller.emplace();

    // 2-4 角度制御用PID（外側ループ）と角速度制御用PID（内側ループ）
    // Note: InitStateの initializeCascadePID で初期化されるため、ここではコメント化
    // 各軸ごとの設定は pid_config.hpp の Pitch/Roll/Yaw 名前空間で管理

    // 2-5 SBUS
    state_context_.instances.sbus_receiver.emplace();

    // 2-6 マネューバーシーケンサー（自動操縦の目標値提供）
    state_context_.instances.maneuver_sequencer.emplace();

    // ISRマネージャにSBUSインスタンスとUARTハンドルを登録
    if (state_context_.instances.sbus_receiver.has_value()) {

    	printf("[Debug] Using SBUS PORT: UART2\n");
        ISRManager::registerSBUS(&state_context_.instances.sbus_receiver.value(), state_context_.pin_config.debug_uart);

        //ISRManager::registerSBUS(&state_context_.instances.sbus_receiver.value(), state_context_.pin_config.sbus_uart);
    }

    // 3. 初期状態を生成
    current_state_ = StateFactory::createState(init_state_id_);


    printf("[StateManager] All Instances Generated\n");
}
