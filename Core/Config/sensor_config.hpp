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
    uint8_t icm42688p_addr = 0x68  // ICM42688p(IMU) デフォルト 0x68
    uint8_t bmm350_addr = 0x14;    // BMM350（磁気） デフォルト 0x14
    uint8_t dps368_addr = 0x77;    // DPS368（気圧・温度） デフォルト 0x77
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
};

#endif // SENSOR_CONFIG_HPP
