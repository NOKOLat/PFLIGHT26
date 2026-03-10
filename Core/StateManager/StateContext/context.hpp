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

#include "SBUS/sbus.h"
#include "sbus_rescaler.hpp"
#include "../../Utility/Sensors/sensor_fusion_manager.hpp"
#include "../../Utility/Motor_Servo/Pwm.hpp"
#include "../../Utility/CascadePID/cascade_pid_manager.hpp"
#include "../../Utility/ManeuverSequencer/maneuver_sequencer.hpp"

// 前方宣言: MissionBase (ポインタでのみ使用)
class MissionBase;



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

// 通信ペリフェラル設定を格納する構造体
struct PinConfiguration {

    I2C_HandleTypeDef*  sensor_i2c  = SensorConfig::i2c_handle;  // センサー用 I2C
    UART_HandleTypeDef* sbus_uart   = &huart5;                   // SBUS用 UART
    UART_HandleTypeDef* debug_uart  = &huart2;                   // デバッグ用 UART
};

// 単位換算定数を格納する構造体
struct UnitConversion {

    static constexpr float DEG_TO_RAD = 3.14159265358979323846f / 180.0f;
    static constexpr float RAD_TO_DEG = 180.0f / 3.14159265358979323846f;

    // SBUS制御値 [-100~100] → サーボ角度 [-90~90 deg] への変換係数
    static constexpr float SBUS_TO_SERVO_DEG = 90.0f / 100.0f;
};

struct Instances {

    // センサー融合マネージャー（姿勢推定・高度推定を統合）
    std::optional<SensorFusionManager> sensor_fusion_manager;

    // 通信インスタンス
    std::optional<nokolat::SBUS> sbus_receiver;

    // PWM制御
    std::optional<PwmManager> pwm_controller;

    // カスケードPIDコントローラ（6つのPID: 角度+レート×3軸）
    std::optional<CascadePIDManager> cascade_pid_manager;

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

    // データコンテナ
    Euler3f attitude;                    // 推定された姿勢角 [deg]
    float altitude = 0.0f;               // 推定された高度 [m]

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
