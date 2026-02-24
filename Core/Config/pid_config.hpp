#ifndef PID_CONFIG_HPP
#define PID_CONFIG_HPP

namespace PidConfig {

    // ===== 角度制御 PID ゲイン初期値 =====
    constexpr float ANGLE_KP = 1.0f;
    constexpr float ANGLE_KI = 0.0f;
    constexpr float ANGLE_KD = 0.0f;

    // ===== 角速度制御 PID ゲイン初期値 =====
    constexpr float RATE_KP = 1.0f;
    constexpr float RATE_KI = 0.0f;
    constexpr float RATE_KD = 0.0f;

} // namespace PidConfig

#endif // PID_CONFIG_HPP
