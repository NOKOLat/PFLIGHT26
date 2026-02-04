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

        printf("[StateManager::changeState] Failed to create state (ID: %d)\n", static_cast<int>(state_id));
        return;
    }

    // 現在の状態の終了処理
    if (current_state_) {

        current_state_->exit(state_context_);
    }

    // 新しい状態に遷移
    current_state_ = std::move(new_state);

    // 新しい状態の開始処理
    if (current_state_) {

        current_state_->enter(state_context_);

        // デバッグ出力
        printf("[StateManager] StateEnter: %d\n", static_cast<int>(current_state_->getStateID()));
    }
}

void StateManager::update() {

    // 初回実行時は初期化を行う
    if (is_first_execution_) {

        init();
        return;
    }

    // SBUSデータの更新
    if (state_context_.instances.sbus_receiver.has_value()) {

        nokolat::SBUS& sbus = state_context_.instances.sbus_receiver.value();
        const nokolat::SBUS_DATA& sbus_data = sbus.getData();

        state_context_.control_input.data = sbus_data.data;
        state_context_.control_input.failsafe = sbus_data.failsafe;
        state_context_.control_input.framelost = sbus_data.framelost;

        // SBUSデータをリスケーリング
        state_context_.rescaled_sbus_data = nokolat::SBUSRescaler::rescale(sbus_data.data);
    }

    // 無線通信データの更新

    // あとでLidarからのデータを受け取る部分を実装する
    //
    //

    // 現在の状態が有効でない場合
    if (!current_state_) {

        printf("[StateManager::update] StateInstance Error\n");
        changeState(StateID::EMERGENCY_STATE);
        return;
    }

	// 現在の状態の更新処理
	StateResult result = current_state_->update(state_context_);

	// 処理に失敗した場合
	if (!result.success){

		printf("[StateManager::update] State Update Failed\n");
		changeState(StateID::EMERGENCY_STATE);
		return;
	}

	// 状態遷移が必要な場合
	if (result.should_transition) {

		// 状態遷移を実行（Factoryの呼び出しはchangeState内で行われる）
		changeState(result.next_state_id);
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

    // 2. 使用するインスタンスの初期化u

    // 2-1 センサーモジュールの初期化
    state_context_.instances.imu_sensor.emplace(state_context_.pin_config.sensor_i2c, 0b1101000);
    // state_context_.instances.mag_sensor.emplace(state_context_.pin_config.sensor_i2c, 0x14); // 試験用の基板にないためコメントアウト
    state_context_.instances.baro_sensor.emplace(state_context_.pin_config.sensor_i2c);

    // 2-2 モーターインスタンスの初期化
    state_context_.instances.left_motor.emplace(state_context_.pin_config.motor_tim[0], state_context_.pin_config.motor_tim_channels[0]);
    state_context_.instances.right_motor.emplace(state_context_.pin_config.motor_tim[1], state_context_.pin_config.motor_tim_channels[1]);

    // 2-3 サーボインスタンスの初期化
    state_context_.instances.elevator_servo.emplace(state_context_.pin_config.servo_tim[0], state_context_.pin_config.servo_tim_channels[0]);
    state_context_.instances.rudder_servo.emplace(state_context_.pin_config.servo_tim[1], state_context_.pin_config.servo_tim_channels[1]);
    state_context_.instances.aileron_servo.emplace(state_context_.pin_config.servo_tim[2], state_context_.pin_config.servo_tim_channels[2]);
    state_context_.instances.drop_servo.emplace(state_context_.pin_config.servo_tim[3], state_context_.pin_config.servo_tim_channels[3]);

    // 2-4 姿勢推定フィルタの初期化(6軸モード: IMUのみ使用、magnetometerなし)
    state_context_.instances.madgwick.emplace();
    state_context_.instances.madgwick->begin(1.0f / (state_context_.loop_time_us / 1000000.0f)); // サンプルレート [Hz]

    // 2-5-1角度制御用PID(kp, ki, kd, dt [秒])
    state_context_.instances.angle_roll_pid.emplace(state_context_.pid_gains.angle_kp, state_context_.pid_gains.angle_ki, state_context_.pid_gains.angle_kd,  state_context_.loop_time_us / 1000000.0f);
    state_context_.instances.angle_pitch_pid.emplace(state_context_.pid_gains.angle_kp, state_context_.pid_gains.angle_ki, state_context_.pid_gains.angle_kd, state_context_.loop_time_us / 1000000.0f);
    state_context_.instances.angle_yaw_pid.emplace(state_context_.pid_gains.angle_kp, state_context_.pid_gains.angle_ki, state_context_.pid_gains.angle_kd, state_context_.loop_time_us / 1000000.0f);

    // 2-5-2角速度制御用PID(kp, ki, kd, dt [秒])
    state_context_.instances.rate_roll_pid.emplace(state_context_.pid_gains.rate_kp, state_context_.pid_gains.rate_ki, state_context_.pid_gains.rate_kd, state_context_.loop_time_us / 1000000.0f);
    state_context_.instances.rate_pitch_pid.emplace(state_context_.pid_gains.rate_kp, state_context_.pid_gains.rate_ki, state_context_.pid_gains.rate_kd, state_context_.loop_time_us / 1000000.0f);
    state_context_.instances.rate_yaw_pid.emplace(state_context_.pid_gains.rate_kp, state_context_.pid_gains.rate_ki, state_context_.pid_gains.rate_kd, state_context_.loop_time_us / 1000000.0f);

    // 2-6 SBUS
    state_context_.instances.sbus_receiver.emplace();

    // ISRマネージャにSBUSインスタンスとUARTハンドルを登録
    if (state_context_.instances.sbus_receiver.has_value()) {

        ISRManager::registerSBUS(&state_context_.instances.sbus_receiver.value(), state_context_.pin_config.sbus_uart);
    }

    // 3. 初期状態を生成
    current_state_ = StateFactory::createState(init_state_id_);

    // 4. 初期状態のenter関数を呼ぶ
    if (current_state_) {

        current_state_->enter(state_context_);
    }
    
    printf("[StateManager] All Instances Generated\n");
}
