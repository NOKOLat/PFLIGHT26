#ifndef MOTOR_SERVO_PWM_HPP
#define MOTOR_SERVO_PWM_HPP

#include "tim.h"  // STM32 HAL Timer handles
#include "../../Config/pwm_config.hpp"
#include "../ServoPwm4f.hpp"
#include "../MotorPwm2f.hpp"
#include "STM32_Motor-Servo_Driver/motor_controller.hpp"
#include "STM32_Motor-Servo_Driver/servo_controller.hpp"

class PwmManager {

    public:

        // コンストラクタでモーター・サーボを初期化
        PwmManager();

        bool initPwm();

        // 構造体を使用したインデックス指定API（推奨）
        bool setServoAngle(const ServoPwm4f& servo_pwm);
        bool setMotorSpeed(const MotorPwm2f& motor_pwm);

        // 従来のfloat配列API（互換性維持）
        bool setServoAngle(const float angle[4]);
        bool setServoAngle_Independent(const float angle[4]);  // テスト用：各サーボを独立制御
        bool setMotorSpeed(const float speed[2]);

    private:

        // instance motor (pwm_config.hpp から参照)
        MotorController right_motor{PwmConfig::right_motor_tim, PwmConfig::RIGHT_MOTOR_CHANNEL};
        MotorController left_motor{PwmConfig::left_motor_tim, PwmConfig::LEFT_MOTOR_CHANNEL};

        // instance servo (pwm_config.hpp から参照)
        ServoController elevator_servo{PwmConfig::elevator_servo_tim, PwmConfig::ELEVATOR_SERVO_CHANNEL};
        ServoController rudder_servo{PwmConfig::rudder_servo_tim, PwmConfig::RUDDER_SERVO_CHANNEL};
        ServoController right_aileron_servo{PwmConfig::right_aileron_servo_tim, PwmConfig::RIGHT_AILERON_SERVO_CHANNEL};
        ServoController left_aileron_servo{PwmConfig::left_aileron_servo_tim, PwmConfig::LEFT_AILERON_SERVO_CHANNEL};

        ServoController drop_servo{PwmConfig::drop_servo_tim, PwmConfig::DROP_SERVO_CHANNEL};

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
