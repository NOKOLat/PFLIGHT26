#ifndef MOTOR_SERVO_PWM_HPP
#define MOTOR_SERVO_PWM_HPP

#include "tim.h"
#include "STM32_Motor-Servo_Driver/motor_controller.hpp"
#include "STM32_Motor-Servo_Driver/servo_controller.hpp"

struct PwmPinConfig{

    TIM_HandleTypeDef* tim;
    uint32_t channel;
};

class PwmManager {

    public:

        // コンストラクタでモーター・サーボを初期化
        PwmManager();

        bool initPwm();
        bool setServoAngle(float angle[4]);
        bool setServoAngle_Independent(float angle[4]);  // テスト用：各サーボを独立制御
        bool setMotorSpeed(float speed[2]);

    private:

        // config: motor
        PwmPinConfig right_motor_config = {&htim3, TIM_CHANNEL_1};
        PwmPinConfig left_motor_config = {&htim3, TIM_CHANNEL_2};
    
        // config: servo
        PwmPinConfig elevator_servo_config = {&htim1, TIM_CHANNEL_1};
        PwmPinConfig rudder_servo_config = {&htim1, TIM_CHANNEL_2};
        PwmPinConfig right_aileron_servo_config = {&htim1, TIM_CHANNEL_3};
        PwmPinConfig left_aileron_servo_config = {&htim1, TIM_CHANNEL_4};
        PwmPinConfig drop_servo_config = {&htim12, TIM_CHANNEL_2};

        // instance motor
        MotorController right_motor;
        MotorController left_motor;

        // instance servo
        ServoController elevator_servo;
        ServoController rudder_servo;
        ServoController right_aileron_servo;
        ServoController left_aileron_servo;
        ServoController drop_servo;

        // パルス幅の範囲設定
        // モーター: 1000～2000us (0% ～ 100%)
        uint32_t motor_min_pulse_us = 1000;
        uint32_t motor_max_pulse_us = 2000;

        // サーボ: 1500～1900us (-90度 ～ +90度) - ES9051データシート値
        uint32_t servo_min_pulse_us = 1000;     // -90度
        uint32_t servo_neutral_pulse_us = 1500; // 0度
        uint32_t servo_max_pulse_us = 2000;     // +90度
};

#endif// MOTOR_SERVO_PWM_HPP
