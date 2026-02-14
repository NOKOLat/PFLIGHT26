#ifndef MOTOR_SERVO_PWM_HPP
#define MOTOR_SERVO_PWM_HPP

#include "tim.h"
#include "STM32_Motor-Servo_Driver/motor_controller.hpp"
#include "STM32_Motor-Servo_Driver/servo_controller.hpp"

class PwmManager {

    public:

        // コンストラクタでモーター・サーボを初期化
        PwmManager();

        bool initPwm();
        bool setServoAngle(float angle[4]);
        bool setMotorSpeed(float speed[2]);

    private:

        MotorController right_motor;
        MotorController left_motor;

        ServoController elevator_servo;
        ServoController rudder_servo;
        ServoController right_aileron_servo;
        ServoController left_aileron_servo;
        ServoController drop_servo;

        // パルス幅の範囲設定
        // モーター: 1000～2000us (0% ～ 100%)
        uint32_t motor_min_pulse_us = 1000;
        uint32_t motor_max_pulse_us = 2000;

        // サーボ: 1600～1900us (-90度 ～ +90度)
        uint32_t servo_min_pulse_us = 1600;     // -90度
        uint32_t servo_neutral_pulse_us = 1750; // 0度 (1600+1900)/2
        uint32_t servo_max_pulse_us = 1900;     // +90度
};

#endif// MOTOR_SERVO_PWM_HPP
