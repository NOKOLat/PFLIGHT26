#ifndef PWM_CONFIG_HPP
#define PWM_CONFIG_HPP

#include <cstdint>
#include "tim.h"  // STM32 HAL Timer handles

namespace PwmConfig {

    // ===== モーター TIM/チャンネル =====
    inline TIM_HandleTypeDef* const right_motor_tim = &htim12;
    constexpr uint32_t RIGHT_MOTOR_CHANNEL = TIM_CHANNEL_1;

    inline TIM_HandleTypeDef* const left_motor_tim = &htim12;
    constexpr uint32_t LEFT_MOTOR_CHANNEL = TIM_CHANNEL_2;

    // ===== サーボ TIM/チャンネル =====
    inline TIM_HandleTypeDef* const elevator_servo_tim = &htim4;
    constexpr uint32_t ELEVATOR_SERVO_CHANNEL = TIM_CHANNEL_1;

    inline TIM_HandleTypeDef* const rudder_servo_tim = &htim4;
    constexpr uint32_t RUDDER_SERVO_CHANNEL = TIM_CHANNEL_2;

    inline TIM_HandleTypeDef* const right_aileron_servo_tim = &htim3;
    constexpr uint32_t RIGHT_AILERON_SERVO_CHANNEL = TIM_CHANNEL_4;

    inline TIM_HandleTypeDef* const left_aileron_servo_tim = &htim2;
    constexpr uint32_t LEFT_AILERON_SERVO_CHANNEL = TIM_CHANNEL_2;

    inline TIM_HandleTypeDef* const drop_servo_tim = &htim3;
    constexpr uint32_t DROP_SERVO_CHANNEL = TIM_CHANNEL_2;

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
