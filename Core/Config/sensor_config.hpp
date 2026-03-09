#ifndef SENSOR_CONFIG_HPP
#define SENSOR_CONFIG_HPP

#include <cstdint>

namespace SensorConfig {

    // ===== IMU センサー設定 =====

    // Z軸反転オプション（基板上でIMUが上下逆に実装されている場合に有効）
    // false: Z軸を反転しない (デフォルト)
    // true:  Z軸を反転する
    constexpr bool ACCEL_INVERT_Z = false;

} // namespace SensorConfig

#endif // SENSOR_CONFIG_HPP
