#ifndef SENSOR_CONFIG_HPP
#define SENSOR_CONFIG_HPP

#include "usart.h"

// すべてのセンサーは1本のi2cで通信できる前提です

/**
 * @struct SensorI2CAddresses
 * @brief センサーの I2C アドレス設定
 *
 * センサーは SDO ピンなどで I2C アドレスを変更可能
 * テスト環境や複数センサーの同時接続時に対応
 */
struct SensorI2CAddresses {
    uint8_t icm42688p_addr = 0x69;  // ICM42688p(IMU) デフォルト 0x68
    uint8_t bmm350_addr = 0x14;    // BMM350（磁気） デフォルト 0x14
    uint8_t dps368_addr = 0x77;    // DPS368（気圧・温度） デフォルト 0x77
};

/**
 * @struct IMUCalibConfig
 * @brief IMU（加速度・ジャイロ）キャリブレーション設定
 *
 * 自動キャリブレーション（enable_auto_calibration = true）か
 * 手動オフセット値（manual_*_offset）かを選択
 */
struct IMUCalibConfig {
    bool     enable_auto_calibration = false;
    int16_t  manual_accel_offset[3]  = {-286, -277, 0};
    int16_t  manual_gyro_offset[3]   = {-5, -8, -37};
};

/**
 * @struct AltitudeCalibConfig
 * @brief 高度推定キャリブレーション設定
 *
 * 気圧センサーから高度を推定する前に、
 * 初期圧力値の取得と加速度スケール調整が必要
 */
struct AltitudeCalibConfig {
    uint32_t sample_count     = 10;       // キャリブレーション回数（サンプル数）
    float    reference_gravity = 9.80665f; // 基準加速度（m/s^2）
};

/**
 * @struct SensorConfig
 * @brief センサー初期化用の設定構造体
 *
 * AttitudeEstimation への DI（依存性注入）に使用
 * StateManager が Config から生成し、AttitudeEstimation に渡す
 */
struct SensorConfig {

    // I2C ハンドル（各種センサーの通信に使用）
    I2C_HandleTypeDef* i2c_handle = nullptr;

    // I2C アドレス設定
    SensorI2CAddresses i2c_addresses;

    // センサーの有効/無効フラグ
    bool enable_imu = true;           // ICM42688P (IMU)
    bool enable_magnetometer = true;  // BMM350 (磁気)
    bool enable_barometer = true;     // DPS368 (気圧・温度)

    // キャリブレーション設定（AttitudeEstimation内で自動実行）
    IMUCalibConfig imu_calib;
    AltitudeCalibConfig altitude_calib;
};

#endif // SENSOR_CONFIG_HPP
