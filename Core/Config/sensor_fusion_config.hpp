#ifndef SENSOR_FUSION_CONFIG_HPP
#define SENSOR_FUSION_CONFIG_HPP

#include <cstdint>

namespace SensorFusionConfig {

    // ===== EKF パラメータ =====
    // 姿勢推定用の拡張カルマンフィルタの設定値
    namespace EKF {

        // プロセスノイズ（状態遷移時のノイズ）
        constexpr float ACCEL_NOISE = 0.1f;        // 加速度センサーノイズ
        constexpr float GYRO_NOISE = 0.05f;        // ジャイロセンサーノイズ
        constexpr float MAG_NOISE = 0.2f;          // 磁気センサーノイズ
    }

    // ===== センサー融合の更新レート =====
    // EKFを実行する周期
    constexpr uint32_t FUSION_UPDATE_RATE_HZ = 100;

} // namespace SensorFusionConfig

#endif // SENSOR_FUSION_CONFIG_HPP
