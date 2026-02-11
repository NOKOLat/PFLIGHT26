#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


void InitState::enter(StateContext& context) {

}

StateResult InitState::update(StateContext& context) {

    StateResult result;

    // 初期化関数の呼び出し
    // 具体的な処理はこのファイルの下に関数ごとに実装されています
    if (!(result = initializeIMU(context)).success) {

        return result;
    }

    if (!(result = initializeMagnetometer(context)).success) {

        return result;
    }

    if (!(result = initializeBarometer(context)).success) {

        return result;
    }

    if (!(result = initializeMotors(context)).success) {

        return result;
    }

    if (!(result = initializeServos(context)).success) {

        return result;
    }

    if (!(result = initializeAttitudeEstimation(context)).success) {

        return result;
    }

    if (!(result = initializeSBUS(context)).success) {

        return result;
    }

    printf("All init complete! \n");
    result.success = true;
    result.should_transition = true;
    result.next_state_id = StateID::CALIBRATION_STATE;

    return result;
}

void InitState::exit(StateContext& context){

}

StateResult InitState::initializeIMU(StateContext& context) {

    // 1-1. IMUの初期化・通信チェック
    if (!context.instances.imu_sensor.has_value()) {

        printf("Error: IMU sensor instance is not initialized.\n");
        return {false, false, StateID::INIT_STATE};
    }

    if (context.instances.imu_sensor->Connection() != 0) {

        printf("Error: IMU sensor connection failed.\n");
        return {false, false, StateID::INIT_STATE};
    }

    // 1-2. IMUの設定
    context.instances.imu_sensor->AccelConfig(ICM42688P::ACCEL_Mode::LowNoize, ICM42688P::ACCEL_SCALE::SCALE02g, ICM42688P::ACCEL_ODR::ODR00500hz, ICM42688P::ACCEL_DLPF::ODR40);
    context.instances.imu_sensor->GyroConfig(ICM42688P::GYRO_MODE::LowNoize, ICM42688P::GYRO_SCALE::Dps0250, ICM42688P::GYRO_ODR::ODR00500hz, ICM42688P::GYRO_DLPF::ODR40);

    return {true, false, StateID::INIT_STATE};
}

StateResult InitState::initializeMagnetometer(StateContext& context) {

    // 1-3. 磁気センサーの初期化・通信チェック
    if (!context.instances.mag_sensor.has_value()) {

        printf("Error: Mag sensor instance is not initialized.\n");
        return {false, false, StateID::INIT_STATE};
    }

    if (context.instances.mag_sensor->init()) {

        printf("Error: Mag sensor connection failed.\n");
        return {false, false, StateID::INIT_STATE};
    }

    // 1-4. 磁気センサーの設定
    context.instances.mag_sensor->config(BMM350_DATA_RATE_400HZ, BMM350_NO_AVERAGING);

    return {true, false, StateID::INIT_STATE};
}

StateResult InitState::initializeBarometer(StateContext& context) {

    // 1-5. 気圧センサーの初期化・通信チェック
    if (!context.instances.baro_sensor.has_value()) {

        printf("Error: Baro sensor instance is not initialized.\n");
        return {false, false, StateID::INIT_STATE};
    }

    if (context.instances.baro_sensor->init() != 0) {

        printf("Error: Baro sensor connection failed.\n");
        return {false, false, StateID::INIT_STATE};
    }

    // 1-6. 気圧センサーの設定
    context.instances.baro_sensor->pressConfig(MEAS_RATE::_128pr_sec, MEAS_SAMPLING::_001_times);
    context.instances.baro_sensor->tempConfig(MEAS_RATE::_128pr_sec, MEAS_SAMPLING::_001_times);

    return {true, false, StateID::INIT_STATE};
}

StateResult InitState::initializeMotors(StateContext& context) {

    // 1-7. モーターの初期化チェック
    if ((context.instances.left_motor.has_value() & context.instances.right_motor.has_value()) != 1) {

        printf("Error: Motor instance is not initialized.\n");
        return {false, false, StateID::INIT_STATE};
    }

    if ((context.instances.left_motor->isInitialized() & context.instances.right_motor->isInitialized()) != 1) {

        printf("Error: Motor instance is not initialized.\n");
        return {false, false, StateID::INIT_STATE};
    }

    return {true, false, StateID::INIT_STATE};
}

StateResult InitState::initializeServos(StateContext& context) {

    // 1-8. サーボの初期化チェック
    if (!context.instances.elevator_servo.has_value() || !context.instances.rudder_servo.has_value() ||
        !context.instances.aileron_servo.has_value() || !context.instances.drop_servo.has_value()) {

        printf("Error: Servo instance is not initialized.\n");
        return {false, false, StateID::INIT_STATE};
    }

    // 1-9. サーボの初期化確認
    if (!context.instances.elevator_servo->isInitialized() || !context.instances.rudder_servo->isInitialized() ||
        !context.instances.aileron_servo->isInitialized() || !context.instances.drop_servo->isInitialized()) {

        printf("Error: Servo instance failed to initialize.\n");
        return {false, false, StateID::INIT_STATE};
    }

    // 1-9-1. サーボの中立位置へ移動
    context.instances.elevator_servo->neutral();
    context.instances.rudder_servo->neutral();
    context.instances.aileron_servo->neutral();
    context.instances.drop_servo->neutral();

    return {true, false, StateID::INIT_STATE};
}

StateResult InitState::initializeAttitudeEstimation(StateContext& context) {

    // 1-10. 姿勢推定の初期化
    if (!context.instances.madgwick.has_value()) {

        printf("Error: Madgwick instance is not initialized.\n");
        return {false, false, StateID::INIT_STATE};
    }

    context.instances.madgwick->begin(1.0f / (context.loop_time_us / 1000000.0f)); // サンプルレート [Hz]

    return {true, false, StateID::INIT_STATE};
}

StateResult InitState::initializeSBUS(StateContext& context) {

    // 1-11. SBUSの受信チェック
    if (!context.instances.sbus_receiver.has_value()) {

        printf("Error: SBUS receiver instance is not initialized.\n");
        return {false, false, StateID::INIT_STATE};
    }

    if (context.rescaled_sbus_data.throttle == 0.0f) {

        printf("Error: SBUS receiver failed to receive data.\n");
        return {false, false, StateID::INIT_STATE};
    } 

    return {true, false, StateID::INIT_STATE};
}

StateID InitState::getStateID() const {

    return StateID::INIT_STATE;
}

StateBaseID InitState::getStateBaseID() const {

    return StateBaseID::INIT_STATE_BASE;
}
