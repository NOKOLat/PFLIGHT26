#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


void InitState::onEnter(StateContext& context) {

}


StateResult InitState::onUpdate(StateContext& context) {

    StateResult result;

    // 初期化関数の呼び出し
    // 具体的な処理はこのファイルの下に関数ごとに実装されています

    // 1. センサーの初期化（SensorManager経由）
    if (!(result = initializeSensors(context)).success) {

        return result;
    }

    // 2. PWM制御（モーター・サーボ）の初期化
    if (!(result = initializePWM(context)).success) {

        return result;
    }

    // 3. 姿勢推定の初期化
    if (!(result = initializeAttitudeEstimation(context)).success) {

        return result;
    }

    // 4. SBUS受信の初期化と確認
    if (!(result = initializeSBUS(context)).success) {

        return result;
    }

    printf("All init complete! \n");
    return result;
}


void InitState::onExit(StateContext& context){

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);

}


StateID InitState::evaluateNextState(StateContext& context) {

    // 初期化完了後はCALIBRATION_STATEに遷移
    return StateID::CALIBRATION_STATE;
}


// 1. センサーの初期化と設定
StateResult InitState::initializeSensors(StateContext& context) {

    // SensorManagerインスタンスチェック
    if (!context.instances.sensor_manager.has_value()) {

        printf("Error: SensorManager instance is not initialized.\n");
        return {false, false, StateID::INIT_STATE};
    }

    // SensorManagerのセンサー初期化
    if (!context.instances.sensor_manager->initSensors()) {

        printf("Error: Sensor initialization failed.\n");
        return {false, false, StateID::INIT_STATE};
    }

    // SensorManagerのセンサー設定
    if (!context.instances.sensor_manager->configSensors()) {

        printf("Error: Sensor configuration failed.\n");
        return {false, false, StateID::INIT_STATE};
    }

    return {true, false, StateID::INIT_STATE};
}

// 2. PWM制御（モーター・サーボ）の初期化
StateResult InitState::initializePWM(StateContext& context) {

    // PwmManagerインスタンスチェック
    if (!context.instances.pwm_controller.has_value()) {

        printf("Error: PWM controller instance is not initialized.\n");
        return {false, false, StateID::INIT_STATE};
    }

    // PwmManagerの初期化
    if (!context.instances.pwm_controller->initPwm()) {

        printf("Error: PWM controller initialization failed.\n");
        return {false, false, StateID::INIT_STATE};
    }

    printf("PWM controller initialized successfully.\n");
    return {true, false, StateID::INIT_STATE};
}

// 3. 姿勢推定の初期化
StateResult InitState::initializeAttitudeEstimation(StateContext& context) {

    // インスタンスチェック
    if (!context.instances.madgwick.has_value()) {

        printf("Error: Madgwick instance is not initialized.\n");
        return {false, false, StateID::INIT_STATE};
    }

    // 初期化
    context.instances.madgwick->begin(1.0f / (context.loop_time_us / 1000000.0f)); // サンプルレート [Hz]

    return {true, false, StateID::INIT_STATE};
}

// 4. SBUS受信の初期化と確認
StateResult InitState::initializeSBUS(StateContext& context) {

    // インスタンスチェック
    if (!context.instances.sbus_receiver.has_value()) {

        printf("Error: SBUS receiver instance is not initialized.\n");
        return {false, false, StateID::INIT_STATE};
    }

//    if (context.rescaled_sbus_data.throttle == 0.0f) {
//
//        printf("Error: SBUS receiver failed to receive data.\n");
//        return {false, false, StateID::INIT_STATE};
//    }

    return {true, false, StateID::INIT_STATE};
}

StateID InitState::getStateID() const {

    return StateID::INIT_STATE;
}
