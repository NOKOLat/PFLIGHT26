#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


void InitState::enter(StateContext& context) {

    // 各インスタンスを emplace で構築（静的メモリ確保）
    context.imu_sensor.emplace();
    context.mag_sensor.emplace();
    context.lidar_sensor.emplace();
    context.barometric_sensor.emplace();
    context.motor_driver.emplace();
    context.pid_controller.emplace();

    // ピン設定の初期化
    context.pin_config.motor_pins[0] = 0;  // PIN_MOTOR_1
    context.pin_config.motor_pins[1] = 1;  // PIN_MOTOR_2
    context.pin_config.motor_pins[2] = 2;  // PIN_MOTOR_3
    context.pin_config.motor_pins[3] = 3;  // PIN_MOTOR_4

    // センサーの初期化
    // context.imu_sensor->initialize();
    // context.mag_sensor->initialize();
    // context.motor_driver->initializePins(context.pin_config);

    // キャリブレーション
    // context.imu_sensor->calibrate();
    // context.mag_sensor->calibrate();
}


StateResult InitState::update(StateContext& context) {

    // 通信チェック等の処理
    // bool is_connected = context.wireless->isConnected();

    StateResult result;
    result.success = true;
    result.should_transition = true;
    result.next_state_id = StateID::CALIBRATION_STATE;

    return result;
}


void InitState::exit(StateContext& context) {

    // クリーンアップ処理
}


StateID InitState::getStateID() const {

    return StateID::INIT_STATE;
}


StateBaseID InitState::getStateBaseID() const {

    return StateBaseID::INIT_STATE_BASE;
}
