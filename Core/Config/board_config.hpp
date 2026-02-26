#ifndef BOARD_CONFIG_HPP
#define BOARD_CONFIG_HPP

// STM32自動生成ヘッダー（extern "C" 済み、C++から安全にインクルード可）
#include "tim.h"
#include "usart.h"
#include "i2c.h"

namespace BoardConfig {

    // ===== 通信ペリフェラル =====
    inline I2C_HandleTypeDef*  const sensor_i2c  = &hi2c1;
    inline UART_HandleTypeDef* const sbus_uart   = &huart5;
    inline UART_HandleTypeDef* const debug_uart  = &huart2;

    // ===== モーター TIM/チャンネル =====
    inline TIM_HandleTypeDef* const right_motor_tim = &htim3;
    constexpr uint32_t RIGHT_MOTOR_CHANNEL = TIM_CHANNEL_4;  // PA6

    inline TIM_HandleTypeDef* const left_motor_tim  = &htim3;
    constexpr uint32_t LEFT_MOTOR_CHANNEL  = TIM_CHANNEL_3;  // PA7

    // ===== サーボ TIM/チャンネル =====
    inline TIM_HandleTypeDef* const elevator_servo_tim      = &htim1;
    constexpr uint32_t ELEVATOR_SERVO_CHANNEL      = TIM_CHANNEL_1;  // PA8

    inline TIM_HandleTypeDef* const rudder_servo_tim        = &htim1;
    constexpr uint32_t RUDDER_SERVO_CHANNEL        = TIM_CHANNEL_2;  // PA9

    inline TIM_HandleTypeDef* const right_aileron_servo_tim = &htim1;
    constexpr uint32_t RIGHT_AILERON_SERVO_CHANNEL = TIM_CHANNEL_3;  // PA10

    inline TIM_HandleTypeDef* const left_aileron_servo_tim  = &htim1;
    constexpr uint32_t LEFT_AILERON_SERVO_CHANNEL  = TIM_CHANNEL_4;  // PA11

    inline TIM_HandleTypeDef* const drop_servo_tim          = &htim12;
    constexpr uint32_t DROP_SERVO_CHANNEL          = TIM_CHANNEL_2;  // PB15

} // namespace BoardConfig

#endif // BOARD_CONFIG_HPP
