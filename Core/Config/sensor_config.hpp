#ifndef SENSOR_CONFIG_HPP
#define SENSOR_CONFIG_HPP

#include <cstdint>
#include "i2c.h"  // STM32 HAL I2C handle

namespace SensorConfig {

    // ===== センサー I2C 設定 =====
    inline I2C_HandleTypeDef* const i2c_handle = &hi2c1;

    // センサー I2C アドレス（7ビット）
    // STM32 HAL では 7ビットアドレスを使用
    // IMU センサー（加速度・ジャイロ）: ICM42688P
    // ICM42688P: デフォルト 0x69、SDO=GND で 0x68 に変更可能
    constexpr uint8_t ICM42688P_ADDR = 0x69;

    // 地磁気センサー: BMM350
    // BMM350: I2C アドレス固定 0x14
    constexpr uint8_t BMM350_ADDR = 0x14;

    // 気圧・温度センサー: DPS368
    // DPS368: デフォルト 0x77、PSA ピンで 0x76 に変更可能
    constexpr uint8_t DPS368_ADDR = 0x77;

    // ===== IMU センサー設定 =====
    // Z軸反転オプション（基板上でIMUが上下逆に実装されている場合に有効）
    // false: Z軸を反転しない (デフォルト)
    // true:  Z軸を反転する
    constexpr bool ACCEL_INVERT_Z = false;

} // namespace SensorConfig

#endif // SENSOR_CONFIG_HPP
