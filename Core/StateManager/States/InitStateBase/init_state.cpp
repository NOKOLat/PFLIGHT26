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

    // SensorManagerインスタンス取得
    SensorManager* sensor_mgr = context.attitude_estimation.getSensorManager();
    if (sensor_mgr == nullptr) {

        printf("Error: SensorManager instance is not initialized.\n");
        return ProcessStatus::FAILURE;
    }

    // SensorManagerのセンサー初期化
    if (!sensor_mgr->initSensors()) {

        printf("Error: Sensor initialization failed.\n");
        return ProcessStatus::FAILURE;
    }

    // SensorManagerのセンサー設定
    if (!sensor_mgr->configSensors()) {

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

    // AttitudeEstimation が初期化されているか確認
    SensorManager* sensor_mgr = context.attitude_estimation.getSensorManager();
    if (sensor_mgr == nullptr) {

        printf("Error: AttitudeEstimation is not properly initialized.\n");
        return ProcessStatus::FAILURE;
    }

    AttitudeEKF_t* ekf = context.attitude_estimation.getAttitudeEKF();
    if (ekf == nullptr) {

        printf("Error: AttitudeEKF is not initialized.\n");
        return ProcessStatus::FAILURE;
    }

    Altitude* alt_est = context.attitude_estimation.getAltitudeEstimator();
    if (alt_est == nullptr) {

        printf("Error: Altitude estimator is not initialized.\n");
        return ProcessStatus::FAILURE;
    }

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

    // === Pitch軸 PID ===
    context.instances.angle_pitch_pid.emplace(
        PidConfig::Pitch::Angle::KP,
        PidConfig::Pitch::Angle::KI,
        PidConfig::Pitch::Angle::KD,
        dt
    );

    context.instances.rate_pitch_pid.emplace(
        PidConfig::Pitch::Rate::KP,
        PidConfig::Pitch::Rate::KI,
        PidConfig::Pitch::Rate::KD,
        dt
    );

    // === Roll軸 PID ===
    context.instances.angle_roll_pid.emplace(
        PidConfig::Roll::Angle::KP,
        PidConfig::Roll::Angle::KI,
        PidConfig::Roll::Angle::KD,
        dt
    );

    context.instances.rate_roll_pid.emplace(
        PidConfig::Roll::Rate::KP,
        PidConfig::Roll::Rate::KI,
        PidConfig::Roll::Rate::KD,
        dt
    );

    // === Yaw軸 PID ===
    context.instances.angle_yaw_pid.emplace(
        PidConfig::Yaw::Angle::KP,
        PidConfig::Yaw::Angle::KI,
        PidConfig::Yaw::Angle::KD,
        dt
    );

    context.instances.rate_yaw_pid.emplace(
        PidConfig::Yaw::Rate::KP,
        PidConfig::Yaw::Rate::KI,
        PidConfig::Yaw::Rate::KD,
        dt
    );

    printf("[InitState] Cascade PID initialized successfully\n");
    printf("  Pitch - Angle: Kp=%.2f, Ki=%.2f, Kd=%.2f | Rate: Kp=%.2f, Ki=%.2f, Kd=%.2f\n",
           PidConfig::Pitch::Angle::KP, PidConfig::Pitch::Angle::KI, PidConfig::Pitch::Angle::KD,
           PidConfig::Pitch::Rate::KP, PidConfig::Pitch::Rate::KI, PidConfig::Pitch::Rate::KD);
    printf("  Roll  - Angle: Kp=%.2f, Ki=%.2f, Kd=%.2f | Rate: Kp=%.2f, Ki=%.2f, Kd=%.2f\n",
           PidConfig::Roll::Angle::KP, PidConfig::Roll::Angle::KI, PidConfig::Roll::Angle::KD,
           PidConfig::Roll::Rate::KP, PidConfig::Roll::Rate::KI, PidConfig::Roll::Rate::KD);
    printf("  Yaw   - Angle: Kp=%.2f, Ki=%.2f, Kd=%.2f | Rate: Kp=%.2f, Ki=%.2f, Kd=%.2f\n",
           PidConfig::Yaw::Angle::KP, PidConfig::Yaw::Angle::KI, PidConfig::Yaw::Angle::KD,
           PidConfig::Yaw::Rate::KP, PidConfig::Yaw::Rate::KI, PidConfig::Yaw::Rate::KD);
    printf("  Control period: %.6f seconds\n", dt);

    return ProcessStatus::SUCCESS;
}

StateID InitState::getStateID() const {

    return StateID::INIT_STATE;
}
