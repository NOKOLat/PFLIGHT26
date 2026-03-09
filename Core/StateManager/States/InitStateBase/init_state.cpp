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

    // 5. カスケードPID制御の初期化（角度制御＋角速度制御）
    if ((result = initializeCascadePID(context)) == ProcessStatus::FAILURE) {

        return result;
    }

    printf("All init complete! \n");

    return result;
}


StateID InitState::evaluateNextState(StateContext& context) {

    // 初期化完了後はCALIBRATION_STATEに遷移
    return StateID::CALIBRATION_STATE;
}


// 1. センサーの初期化と設定
ProcessStatus InitState::initializeSensors(StateContext& context) {

    // SensorFusionManagerインスタンスチェック
    if (!context.instances.sensor_fusion_manager.has_value()) {

        printf("Error: SensorFusionManager instance is not initialized.\n");
        return ProcessStatus::FAILURE;
    }

    // センサーは StateManager の init() で既に初期化されているため、
    // ここではインスタンスの確認のみを行う
    printf("[InitState] Sensor initialization confirmed.\n");

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

    // インスタンスチェック (SensorFusionManagerの初期化はStateManagerで実施済み)
    if (!context.instances.sensor_fusion_manager.has_value()) {

        printf("Error: SensorFusionManager instance is not initialized.\n");
        return ProcessStatus::FAILURE;
    }

    printf("[InitState] Attitude estimation confirmed.\n");
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

// 5. カスケードPID制御の初期化（角度制御＋角速度制御）
ProcessStatus InitState::initializeCascadePID(StateContext& context) {

    // 制御周期をマイクロ秒から秒に変換
    float dt = context.loop_time_us / 1000000.0f;

    // CascadePIDManagerの初期化（6つのPID: 角度+レート×3軸）
    context.instances.cascade_pid_manager.emplace(dt);

    printf("[InitState] Cascade PID initialized successfully\n");
    printf("  Pitch - Angle: Kp=%.2f, Ki=%.2f, Kd=%.2f | Rate: Kp=%.2f, Ki=%.2f, Kd=%.2f\n",
           CascadePidConfig::Pitch::Angle::KP, CascadePidConfig::Pitch::Angle::KI, CascadePidConfig::Pitch::Angle::KD,
           CascadePidConfig::Pitch::Rate::KP, CascadePidConfig::Pitch::Rate::KI, CascadePidConfig::Pitch::Rate::KD);
    printf("  Roll  - Angle: Kp=%.2f, Ki=%.2f, Kd=%.2f | Rate: Kp=%.2f, Ki=%.2f, Kd=%.2f\n",
           CascadePidConfig::Roll::Angle::KP, CascadePidConfig::Roll::Angle::KI, CascadePidConfig::Roll::Angle::KD,
           CascadePidConfig::Roll::Rate::KP, CascadePidConfig::Roll::Rate::KI, CascadePidConfig::Roll::Rate::KD);
    printf("  Yaw   - Angle: Kp=%.2f, Ki=%.2f, Kd=%.2f | Rate: Kp=%.2f, Ki=%.2f, Kd=%.2f\n",
           CascadePidConfig::Yaw::Angle::KP, CascadePidConfig::Yaw::Angle::KI, CascadePidConfig::Yaw::Angle::KD,
           CascadePidConfig::Yaw::Rate::KP, CascadePidConfig::Yaw::Rate::KI, CascadePidConfig::Yaw::Rate::KD);
    printf("  Control period: %.6f seconds\n", dt);

    return ProcessStatus::SUCCESS;
}

StateID InitState::getStateID() const {

    return StateID::INIT_STATE;
}
