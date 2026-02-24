#ifndef MOTOR_SERVO_PWM_HPP
#define MOTOR_SERVO_PWM_HPP

#include "../../Config/board_config.hpp"
#include "../../Config/pwm_config.hpp"
#include "STM32_Motor-Servo_Driver/motor_controller.hpp"
#include "STM32_Motor-Servo_Driver/servo_controller.hpp"

class PwmManager {

    public:

        // コンストラクタでモーター・サーボを初期化
        PwmManager();

        bool initPwm();
        bool setServoAngle(float angle[4]);
        bool setServoAngle_Independent(float angle[4]);  // テスト用：各サーボを独立制御
        bool setMotorSpeed(float speed[2]);

    private:

        // instance motor
        MotorController right_motor{BoardConfig::right_motor_tim, BoardConfig::RIGHT_MOTOR_CHANNEL};
        MotorController left_motor{BoardConfig::left_motor_tim, BoardConfig::LEFT_MOTOR_CHANNEL};

        // instance servo
        ServoController elevator_servo{BoardConfig::elevator_servo_tim, BoardConfig::ELEVATOR_SERVO_CHANNEL};
        ServoController rudder_servo{BoardConfig::rudder_servo_tim, BoardConfig::RUDDER_SERVO_CHANNEL};
        ServoController right_aileron_servo{BoardConfig::right_aileron_servo_tim, BoardConfig::RIGHT_AILERON_SERVO_CHANNEL};
        ServoController left_aileron_servo{BoardConfig::left_aileron_servo_tim, BoardConfig::LEFT_AILERON_SERVO_CHANNEL};
        ServoController drop_servo{BoardConfig::drop_servo_tim, BoardConfig::DROP_SERVO_CHANNEL};

        // パルス幅の範囲設定
        // モーター: 1000～2000us (0% ～ 100%)
        uint32_t motor_min_pulse_us     = PwmConfig::MOTOR_MIN_PULSE_US;
        uint32_t motor_max_pulse_us     = PwmConfig::MOTOR_MAX_PULSE_US;

        // サーボ: 1500～1900us (-90度 ～ +90度) - ES9051データシート値
        uint32_t servo_min_pulse_us     = PwmConfig::SERVO_MIN_PULSE_US;
        uint32_t servo_neutral_pulse_us = PwmConfig::SERVO_NEUTRAL_PULSE_US;
        uint32_t servo_max_pulse_us     = PwmConfig::SERVO_MAX_PULSE_US;
};

#endif// MOTOR_SERVO_PWM_HPP
