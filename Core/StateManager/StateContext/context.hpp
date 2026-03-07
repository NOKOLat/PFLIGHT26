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

// config
#include "../../Config/board_config.hpp"
#include "../../Config/pid_config.hpp"
#include "../../Utility/Vector3f.hpp"
#include "../../Utility/Euler3f.hpp"
#include "../../Utility/ServoPwm4f.hpp"
#include "../../Utility/MotorPwm2f.hpp"


#include "1DoF_PID/PID.h"
#include "SBUS/sbus.h"
#include "sbus_rescaler.hpp"
//#include "IMU_EKF/attitude_ekf.h"
#include "../../Utility/Motor_Servo/Pwm.hpp"
#include "../../Utility/ManeuverSequencer/maneuver_sequencer.hpp"
#include "../../Utility/ManeuverSequencer/Missions/missions.hpp"
#include "../../Utility/Sensors/AttitudeEstimation/attitude_estimation.hpp"


// 制御出力を格納する構造体
struct ControlOutput {

    MotorPwm2f motor_pwm;           // 2つのモーターの PWM 値 [0-100] % （右、左）
    ServoPwm4f servo_pwm;           // 4つのサーボの 角度 [-90 ~ 90] deg （エレベーター、ラダー、エルロン、投下装置）
};

// オペレータからの制御入力を格納する構造体
struct ControlInput {

    std::array<uint16_t, 18> data = {};
    bool failsafe = false;
    bool framelost = false;

    uint32_t sbus_failsafe_count = 0;
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

    // SBUS制御値 [-100~100] → サーボ角度 [-90~90 deg] への変換係数
    static constexpr float SBUS_TO_SERVO_DEG = 90.0f / 100.0f;
};

struct Instances {

    // 通信インスタンス
    std::optional<nokolat::SBUS> sbus_receiver;

    // PWM制御
    std::optional<PwmManager> pwm_controller;

    // PID コントローラ
    std::optional<PID> angle_pitch_pid;
    std::optional<PID> angle_roll_pid;
    std::optional<PID> angle_yaw_pid;

    std::optional<PID> rate_pitch_pid;
    std::optional<PID> rate_roll_pid;
    std::optional<PID> rate_yaw_pid;

    // マネューバーシーケンサー（自動操縦の目標値提供）
    std::optional<ManeuverSequencer> maneuver_sequencer;
};

// 状態実行時に必要なすべての情報を包含する構造体
struct StateContext {

    // ピン設定
    PinConfiguration pin_config;
    Instances instances;

    // 単位換算定数
    UnitConversion unit_conversion;

    // 姿勢推定（生データ処理と姿勢推定を内部で行う）
    AttitudeEstimation attitude_estimation;

    // データコンテナ
    AttitudeState attitude_state;        // 姿勢推定結果（sensor_pipelineから取得）
    ControlInput control_input;          // 制御入力 (SBUS生データ)
    nokolat::RescaledSBUSData rescaled_sbus_data; // リスケール済みSBUSデータ
    ControlOutput control_output;        // 制御出力

    // 初期値オフセット（ミッション開始時に記録）
    float initial_yaw_offset = 0.0f;           // ミッション開始時のヨー角平均値
    float initial_altitude_offset = 0.0f;      // ミッション開始時の高度平均値

    // 現在実行中のミッション（PreAutoFlightState でセット）
    const MissionBase* current_mission = nullptr;

    uint32_t loop_time_us = 0;
};

#endif // CONTEXT_HPP
