#ifndef SENSOR_I2C_CONFIG_HPP
#define SENSOR_I2C_CONFIG_HPP

#include <cstdint>

namespace SensorI2CConfig {

    // ===== センサー I2C アドレス（7ビット） =====
    // STM32 HAL では 7ビットアドレスを使用
    // I2C デバイスによっては SDO ピンで変更可能

    // IMU センサー（加速度・ジャイロ）
    // ICM42688P: デフォルト 0x69、SDO=GND で 0x68 に変更可能
    constexpr uint8_t ICM42688P_ADDR = 0x69;

    // 地磁気センサー
    // BMM350: I2C アドレス固定 0x14
    constexpr uint8_t BMM350_ADDR = 0x14;

    // 気圧・温度センサー
    // DPS368: デフォルト 0x77、PSA ピンで 0x76 に変更可能
    constexpr uint8_t DPS368_ADDR = 0x77;

} // namespace SensorI2CConfig

#endif // SENSOR_I2C_CONFIG_HPP
