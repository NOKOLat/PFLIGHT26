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
#include "Vector3f.hpp"

#include "1DoF_PID/PID.h"
#include "SBUS/sbus.h"
#include "sbus_rescaler.hpp"
#include "STM32_BMM350/BMM350_Class.hpp"
#include "STM32_ICM42688P/ICM42688P_HAL_I2C.h"
#include "STM32_DPS368/DPS368_HAL_I2C.hpp"
#include "STM32_Motor-Servo_Driver/motor_controller.hpp"
#include "STM32_Motor-Servo_Driver/servo_controller.hpp"
#include "MadgwickAHRS/src/MadgwickAHRS.h"

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
    // SBUSデータ
    std::array<uint16_t, 18> data = {};
    bool failsafe = false;
    bool framelost = false;
};

struct PIDGains {

    float angle_kp = 1.0f;
    float angle_ki = 0.0f;
    float angle_kd = 0.0f;

    float rate_kp = 1.0f;
    float rate_ki = 0.0f;
    float rate_kd = 0.0f;

    //dtはStateManagerのLoopManagerの値を使用する
};

// ピン設定情報を格納する構造体
struct PinConfiguration {

    I2C_HandleTypeDef* sensor_i2c = &hi2c1;  // センサー用 I2C
    UART_HandleTypeDef* sbus_uart = &huart5; // SBUS用 UART

    // モーター用のTIMとチャンネル(左、右)
    std::array<TIM_HandleTypeDef*, 2> motor_tim= {&htim1, &htim1};
    std::array<uint32_t, 2> motor_tim_channels = {TIM_CHANNEL_1, TIM_CHANNEL_2}; 

    // サーボ用のTIMとチャンネル(エレベーター、ラダー、エルロン、投下装置)
    std::array<TIM_HandleTypeDef*, 4> servo_tim = {&htim3, &htim3, &htim3, &htim1};
    std::array<uint32_t, 4> servo_tim_channels = {TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_1};
};

struct Instances {

    // センサーインスタンス
    std::optional<ICM42688P_HAL_I2C> imu_sensor;
    std::optional<BMM350> mag_sensor;
    std::optional<DPS368_HAL_I2C> baro_sensor;

    // 通信インスタンス
    std::optional<nokolat::SBUS> sbus_receiver;

    // AHRSインスタンス
    std::optional<Madgwick> madgwick;

    // モーター・サーボドライバーインスタンス
    std::optional<MotorController> left_motor;
    std::optional<MotorController> right_motor;
    std::optional<ServoController> elevator_servo;
    std::optional<ServoController> rudder_servo;
    std::optional<ServoController> aileron_servo;
    std::optional<ServoController> drop_servo;

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
