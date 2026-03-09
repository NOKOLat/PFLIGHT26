#ifndef ALTITUDE_ESTIMATOR_CONFIG_HPP
#define ALTITUDE_ESTIMATOR_CONFIG_HPP

#include <cstdint>

namespace AltitudeEstimatorConfig {

    // ===== カルマンフィルタ パラメータ =====
    // 高度推定用のカルマンフィルタの設定値
    namespace KalmanFilter {

        // プロセスノイズ（状態遷移時のノイズ）
        constexpr float BARO_NOISE = 0.5f;         // 気圧センサーノイズ
        constexpr float ACCEL_NOISE = 0.1f;        // 加速度センサーノイズ（垂直成分）
    }

    // ===== 高度推定の更新レート =====
    // カルマンフィルタを実行する周期
    constexpr uint32_t ALTITUDE_UPDATE_RATE_HZ = 50;

} // namespace AltitudeEstimatorConfig

#endif // ALTITUDE_ESTIMATOR_CONFIG_HPP
