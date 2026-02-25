#ifndef PWM_CONFIG_HPP
#define PWM_CONFIG_HPP

#include <cstdint>

namespace PwmConfig {

    // ===== サーボ チャンネル定義 =====
    // control_output.servo_pwm配列のインデックス
    enum class ServoChannel : uint8_t {
        AILERON_L    = 0,        // ch1 エルロン（左）
        RUDDER       = 1,        // ch2 ラダー
        ELEVATOR     = 2,        // ch3 エレベーター
        DROP         = 3,        // ch5 投下装置
        AILERON_R    = 4         // ch6 エルロン（右）
    };

    // ===== モーター チャンネル定義 =====
    // control_output.motor_pwm配列のインデックス
    enum class MotorChannel : uint8_t {
        RIGHT = 0,               // 右モーター
        LEFT  = 1                // 左モーター
    };

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
