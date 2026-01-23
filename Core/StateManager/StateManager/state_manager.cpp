#include "StateManager/state_manager.hpp"
#include "StateFactory/state_factory.hpp"


// コンストラクタ
StateManager::StateManager(StateID init_state_id) : loop_manager_(100000), init_state_id_(init_state_id) {

    // コンストラクタでは初期状態を保存するのみ
    // 実際の初期化はupdate()の初回実行時にinit()で行う
}


void StateManager::changeState(std::unique_ptr<StateInterface> new_state) {

    // 現在の状態の終了処理
    if (current_state_) {

        current_state_->exit(state_context_);

        // デバッグ出力
        // printf("[StateManager] StateExit: %d\n", static_cast<int>(current_state_->getStateID()));
    }

    // 新しい状態に遷移
    current_state_ = std::move(new_state);

    // 新しい状態の開始処理
    if (current_state_) {

        current_state_->enter(state_context_);

        // デバッグ出力
        // printf("[StateManager] StateEnter: %d\n", static_cast<int>(current_state_->getStateID()));
    }
}

void StateManager::update() {

    // 初回実行時は初期化を行う

    if (is_first_execution_) {

        init();
    }

    // ループ間隔まで待機が必要な場合は処理を進めない
    if (loop_manager_.isWaitNextLoop()) {

        return;
    }

    // ループタイムを保存
    loop_time_us_ = loop_manager_.getLoopTime();
    state_context_.timestamp_ms = loop_time_us_ / 1000;

    // 現在の状態が有効な場合
    if (current_state_) {

        // 現在の状態の更新処理
        StateResult result = current_state_->update(state_context_);

        // 状態遷移が必要かつ処理が成功した場合
        if (result.should_transition && result.success) {

            // 新しい状態を生成
            std::unique_ptr<StateInterface> next_state = StateFactory::createState(result.next_state_id);

            // 状態遷移を実行
            if (next_state) {

                changeState(std::move(next_state));
            }
        }
    }
}

// 初期化処理
// インスタンスの初期化、初期状態の設定を行う
void StateManager::init() {

    // 初回実行時のみ実行
    if (!is_first_execution_) {

        return;
    }

    // 1. 初回実行フラグをクリア
    is_first_execution_ = false;

    // 2. 使用するインスタンスの初期化

    // 2-1 センサーモジュールの初期化
    state_context_.instances.imu_sensor.emplace(state_context_.pin_config.sensor_i2c);
    state_context_.instances.mag_sensor.emplace(state_context_.pin_config.sensor_i2c, 0x14);
    state_context_.instances.baro_sensor.emplace(state_context_.pin_config.sensor_i2c);

    // 2-2 モーターインスタンスの初期化
    state_context_.instances.left_motor.emplace(state_context_.pin_config.motor_tim[0], state_context_.pin_config.motor_tim_channels[0]);
    state_context_.instances.right_motor.emplace(state_context_.pin_config.motor_tim[1], state_context_.pin_config.motor_tim_channels[1]);

    // 2-3 サーボインスタンスの初期化
    state_context_.instances.elevator_servo.emplace(state_context_.pin_config.servo_tim[0], state_context_.pin_config.servo_tim_channels[0]);
    state_context_.instances.rudder_servo.emplace(state_context_.pin_config.servo_tim[1], state_context_.pin_config.servo_tim_channels[1]);
    state_context_.instances.aileron_servo.emplace(state_context_.pin_config.servo_tim[2], state_context_.pin_config.servo_tim_channels[2]);
    state_context_.instances.drop_servo.emplace(state_context_.pin_config.servo_tim[3], state_context_.pin_config.servo_tim_channels[3]);

    // 2-4 姿勢推定フィルタの初期化

    // ライブラリの修正中

    // 2-5-1角度制御用PID(kp, ki, kd, dt [秒])
    state_context_.instances.angle_roll_pid.emplace(state_context_.pid_gains.angle_kp, state_context_.pid_gains.angle_ki, state_context_.pid_gains.angle_kd, loop_time_us_ / 1000000.0f);
    state_context_.instances.angle_pitch_pid.emplace(state_context_.pid_gains.angle_kp, state_context_.pid_gains.angle_ki, state_context_.pid_gains.angle_kd, loop_time_us_ / 1000000.0f);
    state_context_.instances.angle_yaw_pid.emplace(state_context_.pid_gains.angle_kp, state_context_.pid_gains.angle_ki, state_context_.pid_gains.angle_kd, loop_time_us_ / 1000000.0f);

    // 2-5-2角速度制御用PID(kp, ki, kd, dt [秒])
    state_context_.instances.rate_roll_pid.emplace(state_context_.pid_gains.rate_kp, state_context_.pid_gains.rate_ki, state_context_.pid_gains.rate_kd, loop_time_us_ / 1000000.0f);
    state_context_.instances.rate_pitch_pid.emplace(state_context_.pid_gains.rate_kp, state_context_.pid_gains.rate_ki, state_context_.pid_gains.rate_kd, loop_time_us_ / 1000000.0f);
    state_context_.instances.rate_yaw_pid.emplace(state_context_.pid_gains.rate_kp, state_context_.pid_gains.rate_ki, state_context_.pid_gains.rate_kd, loop_time_us_ / 1000000.0f);

    // 2-6 SBUS
    state_context_.instances.sbus_receiver.emplace();

    // 3. 初期状態を生成
    current_state_ = StateFactory::createState(init_state_id_);

    // 4. 初期状態のenter関数を呼ぶ
    if (!current_state_) {

        return;
    }
    
    current_state_->enter(state_context_);
}
