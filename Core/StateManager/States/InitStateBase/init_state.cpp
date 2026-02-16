#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


ProcessStatus InitState::onUpdate(StateContext& context) {

    ProcessStatus result;

    // 初期化関数の呼び出し
    // 具体的な処理はこのファイルの下に関数ごとに実装されています

    // 1. センサーの初期化（SensorManager経由）
    if ((result = initializeSensors(context)) == ProcessStatus::FAILURE) {

        return result;
    }

    // 2. PWM制御（モーター・サーボ）の初期化
    if ((result = initializePWM(context)) == ProcessStatus::FAILURE) {

        return result;
    }

    // 3. 姿勢推定の初期化
    if ((result = initializeAttitudeEstimation(context)) == ProcessStatus::FAILURE) {

        return result;
    }

    // 4. SBUS受信の初期化と確認
    if ((result = initializeSBUS(context)) == ProcessStatus::FAILURE) {

        return result;
    }

    printf("All init complete! \n");

    // GPIO設定
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);

    return result;
}


StateID InitState::evaluateNextState(StateContext& context) {

    // 初期化完了後はCALIBRATION_STATEに遷移
    return StateID::CALIBRATION_STATE;
}


// 1. センサーの初期化と設定
ProcessStatus InitState::initializeSensors(StateContext& context) {

    // SensorManagerインスタンスチェック
    if (!context.instances.sensor_manager.has_value()) {

        printf("Error: SensorManager instance is not initialized.\n");
        return ProcessStatus::FAILURE;
    }

    // SensorManagerのセンサー初期化
    if (!context.instances.sensor_manager->initSensors()) {

        printf("Error: Sensor initialization failed.\n");
        return ProcessStatus::FAILURE;
    }

    // SensorManagerのセンサー設定
    if (!context.instances.sensor_manager->configSensors()) {

        printf("Error: Sensor configuration failed.\n");
        return ProcessStatus::FAILURE;
    }

    return ProcessStatus::SUCCESS;
}

// 2. PWM制御（モーター・サーボ）の初期化
ProcessStatus InitState::initializePWM(StateContext& context) {

    // PwmManagerインスタンスチェック
    if (!context.instances.pwm_controller.has_value()) {

        printf("Error: PWM controller instance is not initialized.\n");
        return ProcessStatus::FAILURE;
    }

    // PwmManagerの初期化
    if (!context.instances.pwm_controller->initPwm()) {

        printf("Error: PWM controller initialization failed.\n");
        return ProcessStatus::FAILURE;
    }

    printf("PWM controller initialized successfully.\n");
    return ProcessStatus::SUCCESS;
}

// 3. 姿勢推定の初期化
ProcessStatus InitState::initializeAttitudeEstimation(StateContext& context) {

    // インスタンスチェック
    if (!context.instances.madgwick.has_value()) {

        printf("Error: Madgwick instance is not initialized.\n");
        return ProcessStatus::FAILURE;
    }

    // 初期化
    context.instances.madgwick->begin(1.0f / (context.loop_time_us / 1000000.0f)); // サンプルレート [Hz]

    return ProcessStatus::SUCCESS;
}

// 4. SBUS受信の初期化と確認
ProcessStatus InitState::initializeSBUS(StateContext& context) {

    // インスタンスチェック
    if (!context.instances.sbus_receiver.has_value()) {

        printf("Error: SBUS receiver instance is not initialized.\n");
        return ProcessStatus::FAILURE;
    }

//    if (context.rescaled_sbus_data.throttle == 0.0f) {
//
//        printf("Error: SBUS receiver failed to receive data.\n");
//        return ProcessStatus::FAILURE;
//    }

    return ProcessStatus::SUCCESS;
}

StateID InitState::getStateID() const {

    return StateID::INIT_STATE;
}
