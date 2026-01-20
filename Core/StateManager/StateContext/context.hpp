#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <cstdint>
#include <array>
#include <optional>
#include "Vector3f.hpp"


// 前方宣言（実装は Lib 内）
class IMUSensor;
class MagnetometerSensor;
class MotorDriver;
class PIDController;
class LiDARSensor;
class BarometricSensor;

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
    float altitude;       // 高度 [m]
    float barometric_pressure; // 気圧 [Pa]
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

    // 4つのモーターの PWM 値 [0-1000]
    uint16_t motor_pwm[4];
};


// ピン設定情報を格納する構造体
struct PinConfiguration {

    uint16_t motor_pins[4];    // モーターのピン番号
    uint16_t servo_pins[2];    // サーボのピン番号
    uint16_t sensor_pins[4];   // センサーのピン番号
};


// 状態実行時に必要なすべての情報を包含する構造体
struct StateContext {

    // ピン設定
    PinConfiguration pin_config;

    // 共有データ
    ControlOutput control_output;
    SensorData sensor_data;
    AttitudeState attitude;

    // タイムスタンプ
    uint32_t timestamp_ms;
};

#endif // CONTEXT_HPP
