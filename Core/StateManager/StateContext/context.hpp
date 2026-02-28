#ifndef CONTEXT_HPP
#define CONTEXT_HPP

// MatrixX.tppの二重インクルード防止ガード（最優先）
#ifndef MATRIXX_TPP_INCLUDED
#define MATRIXX_TPP_INCLUDED
#endif

#include <cstdint>
#include <array>
#include <optional>
#include "usart.h"
#include "../../Config/board_config.hpp"
#include "../../Config/pid_config.hpp"
#include "../../Utility/Vector3f.hpp"

#include "1DoF_PID/PID.h"
#include "SBUS/sbus.h"
#include "sbus_rescaler.hpp"
#include "IMU_EKF/attitude_ekf.h"
#include "Altitude_estimation/altitude.h"
#include "../../Utility/Sensors/SensorManager.hpp"
#include "../../Utility/Motor_Servo/Pwm.hpp"

// センサーデータを格納する構造体
struct SensorData {

    // IMU (ICM42688P)
    Vector3f accel;      // 加速度 [m/s^2]
    Vector3f gyro;       // 角速度 [rad/s]

    // 磁気センサー (BMM350)
    Vector3f mag;        // 磁気 [uT]

    // LiDAR
    Vector3f lidar_coord; // LiDARからの座標 [m]

    // 気圧センサー (DPS368)
    float barometric_pressure; // 気圧 [Pa]
    float temperature;    // 温度 [℃]

    // 計算データ
    Vector3f angle;     // 角度 [deg]
    float altitude;       // 高度 [m]
    float altitude_velocity;    // 高度速度 [m/s]
    float altitude_accel;       // 高度加速度 [m/s^2]
};


// 姿勢推定結果を格納する構造体
struct AttitudeState {

    float roll;           // ロール角 [deg]
    float pitch;          // ピッチ角 [deg]
    float yaw;            // ヨー角 [deg]
    float roll_rate;      // ロール角速度 [deg/s]
    float pitch_rate;     // ピッチ角速度 [deg/s]
    float yaw_rate;       // ヨー角速度 [deg/s]
};


// 制御出力を格納する構造体
struct ControlOutput {

    std::array<float, 2> motor_pwm; // 2つのモーターの PWM 値 [0-100] % （右、左）
    std::array<float, 4> servo_pwm; // 4つのサーボの 角度 [-90 ~ 90] deg （エレベーター、ラダー、エルロン、投下装置）
};

// オペレータからの制御入力を格納する構造体
struct ControlInput {

    std::array<uint16_t, 18> data = {};
    bool failsafe = false;
    bool framelost = false;

    uint32_t sbus_failsafe_count = 0;
};

struct PIDGains {

    float angle_kp = PidConfig::ANGLE_KP;
    float angle_ki = PidConfig::ANGLE_KI;
    float angle_kd = PidConfig::ANGLE_KD;

    float rate_kp = PidConfig::RATE_KP;
    float rate_ki = PidConfig::RATE_KI;
    float rate_kd = PidConfig::RATE_KD;

    //dtはStateManagerのLoopManagerの値を使用する
};

// ピン設定情報を格納する構造体
struct PinConfiguration {

    I2C_HandleTypeDef*  sensor_i2c  = BoardConfig::sensor_i2c;  // センサー用 I2C
    UART_HandleTypeDef* sbus_uart   = BoardConfig::sbus_uart;   // SBUS用 UART
    UART_HandleTypeDef* debug_uart  = BoardConfig::debug_uart;  // デバッグ用 UART

    // モーター用のTIMとチャンネル(右、左)
    std::array<TIM_HandleTypeDef*, 2> motor_tim = {
        BoardConfig::right_motor_tim,
        BoardConfig::left_motor_tim
    };
    std::array<uint32_t, 2> motor_tim_channels = {
        BoardConfig::RIGHT_MOTOR_CHANNEL,
        BoardConfig::LEFT_MOTOR_CHANNEL
    };

    // サーボ用のTIMとチャンネル(エレベーター、ラダー、エルロン右、エルロン左)
    std::array<TIM_HandleTypeDef*, 4> servo_tim = {
        BoardConfig::elevator_servo_tim,
        BoardConfig::rudder_servo_tim,
        BoardConfig::right_aileron_servo_tim,
        BoardConfig::left_aileron_servo_tim
    };
    std::array<uint32_t, 4> servo_tim_channels = {
        BoardConfig::ELEVATOR_SERVO_CHANNEL,
        BoardConfig::RUDDER_SERVO_CHANNEL,
        BoardConfig::RIGHT_AILERON_SERVO_CHANNEL,
        BoardConfig::LEFT_AILERON_SERVO_CHANNEL
    };
};

// 単位換算定数を格納する構造体
struct UnitConversion {

    static constexpr float DEG_TO_RAD = 3.14159265358979323846f / 180.0f;
    static constexpr float RAD_TO_DEG = 180.0f / 3.14159265358979323846f;
};

struct Instances {

    // センサーマネージャー
    std::optional<SensorManager> sensor_manager;

    // 通信インスタンス
    std::optional<nokolat::SBUS> sbus_receiver;

    // 姿勢推定EKFインスタンス
    std::optional<AttitudeEKF_t> attitude_ekf;

    // 高度推定インスタンス
    std::optional<Altitude> altitude_estimator;

    // PWM制御
    std::optional<PwmManager> pwm_controller;

    // PID コントローラ
    std::optional<PID> angle_pitch_pid;
    std::optional<PID> angle_roll_pid;
    std::optional<PID> angle_yaw_pid;

    std::optional<PID> rate_pitch_pid;
    std::optional<PID> rate_roll_pid;
    std::optional<PID> rate_yaw_pid;
};

// 状態実行時に必要なすべての情報を包含する構造体
struct StateContext {

    // ピン設定
    PinConfiguration pin_config;
    Instances instances;

    // 単位換算定数
    UnitConversion unit_conversion;

    // データコンテナ
    SensorData sensor_data;              // センサー生データ
    AttitudeState attitude_state;        // 姿勢推定結果
    ControlInput control_input;          // 制御入力 (SBUS生データ)
    nokolat::RescaledSBUSData rescaled_sbus_data; // リスケール済みSBUSデータ
    ControlOutput control_output;        // 制御出力
    PIDGains pid_gains;                  // PIDゲイン

    uint32_t loop_time_us = 0;    
};

#endif // CONTEXT_HPP
