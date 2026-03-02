#ifndef PWM_CONFIG_HPP
#define PWM_CONFIG_HPP

#include <cstdint>

namespace PwmConfig {

    // ===== モーター PWM パルス幅設定 =====
    // モーター: 1000~2000us (0% ~ 100%)
    constexpr uint32_t MOTOR_MIN_PULSE_US  = 1000;
    constexpr uint32_t MOTOR_MAX_PULSE_US  = 2000;

    // ===== サーボ PWM パルス幅設定 =====
    // サーボ: ES9051データシート値 (-90度 ~ +90度)
    constexpr uint32_t SERVO_MIN_PULSE_US     = 1000;  // -90度
    constexpr uint32_t SERVO_NEUTRAL_PULSE_US = 1500;  //   0度
    constexpr uint32_t SERVO_MAX_PULSE_US     = 2000;  // +90度

} // namespace PwmConfig

#endif // PWM_CONFIG_HPP
