#ifndef CALIBRATION_CONFIG_HPP
#define CALIBRATION_CONFIG_HPP

namespace CalibrationConfig {

    // ===== キャリブレーション有効フラグ =====
    // true: センサーキャリブレーションを実行
    // false: 手動設定値を使用（キャリブレーション実行をスキップ）
    constexpr bool ENABLE_CALIBRATION = false;

    // ===== 手動設定用オフセット値 =====
    // ENABLE_CALIBRATION = false の場合に使用される値
    // これらの値は、水平な面に置いた状態での実測値から設定します

    // 加速度センサーオフセット値（int16）
    constexpr int16_t MANUAL_ACCEL_OFFSET_X = -286;
    constexpr int16_t MANUAL_ACCEL_OFFSET_Y = -277;
    constexpr int16_t MANUAL_ACCEL_OFFSET_Z = 0;

    // ジャイロセンサーオフセット値（int16）
    constexpr int16_t MANUAL_GYRO_OFFSET_X = -5;
    constexpr int16_t MANUAL_GYRO_OFFSET_Y = -8;
    constexpr int16_t MANUAL_GYRO_OFFSET_Z = -37;

} // namespace CalibrationConfig

#endif // CALIBRATION_CONFIG_HPP
