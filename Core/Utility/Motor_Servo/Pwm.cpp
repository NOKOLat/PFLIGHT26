#include "Pwm.hpp"
#include "stdio.h"

// ピン設定はPwm.hppのprivateセクションで定義
PwmManager::PwmManager()
    : right_motor(right_motor_config.tim, right_motor_config.channel),
      left_motor(left_motor_config.tim, left_motor_config.channel),
      elevator_servo(elevator_servo_config.tim, elevator_servo_config.channel),
      rudder_servo(rudder_servo_config.tim, rudder_servo_config.channel),
      right_aileron_servo(right_aileron_servo_config.tim, right_aileron_servo_config.channel),
      left_aileron_servo(left_aileron_servo_config.tim, left_aileron_servo_config.channel),
      drop_servo(drop_servo_config.tim, drop_servo_config.channel) {

    printf("[PwmManager] Constructor called\n");
}

bool PwmManager::initPwm() {

    printf("[PwmManager::initPwm] Starting PWM initialization\n");

    // モーターの初期化確認
    if (!right_motor.isInitialized()) {

        printf("[PwmManager::initPwm] Right motor initialization failed\n");
        return false;
    }

    if (!left_motor.isInitialized()) {

        printf("[PwmManager::initPwm] Left motor initialization failed\n");
        return false;
    }

    // サーボの初期化確認
    if (!elevator_servo.isInitialized()) {

        printf("[PwmManager::initPwm] Elevator servo initialization failed\n");
        return false;
    }

    if (!rudder_servo.isInitialized()) {

        printf("[PwmManager::initPwm] Rudder servo initialization failed\n");
        return false;
    }

    if (!right_aileron_servo.isInitialized()) {

        printf("[PwmManager::initPwm] Right aileron servo initialization failed\n");
        return false;
    }

    if (!left_aileron_servo.isInitialized()) {

        printf("[PwmManager::initPwm] Left aileron servo initialization failed\n");
        return false;
    }

    if (!drop_servo.isInitialized()) {

        printf("[PwmManager::initPwm] Drop servo initialization failed\n");
        return false;
    }

    // すべてのサーボとモーターに範囲を設定
    elevator_servo.setPulseRange(servo_min_pulse_us, servo_max_pulse_us);
    rudder_servo.setPulseRange(servo_min_pulse_us, servo_max_pulse_us);
    right_aileron_servo.setPulseRange(servo_min_pulse_us, servo_max_pulse_us);
    left_aileron_servo.setPulseRange(servo_min_pulse_us, servo_max_pulse_us);
    drop_servo.setPulseRange(servo_min_pulse_us, servo_max_pulse_us);

    right_motor.setPulseRange(motor_min_pulse_us, motor_max_pulse_us);
    left_motor.setPulseRange(motor_min_pulse_us, motor_max_pulse_us);

    // すべてのサーボをニュートラル位置（0度）に設定
    if (elevator_servo.neutral() != 0) {

        printf("[PwmManager::initPwm] Failed to set elevator servo to neutral\n");
        return false;
    }

    if (rudder_servo.neutral() != 0) {

        printf("[PwmManager::initPwm] Failed to set rudder servo to neutral\n");
        return false;
    }

    if (right_aileron_servo.neutral() != 0) {

        printf("[PwmManager::initPwm] Failed to set right aileron servo to neutral\n");
        return false;
    }

    if (left_aileron_servo.neutral() != 0) {

        printf("[PwmManager::initPwm] Failed to set left aileron servo to neutral\n");
        return false;
    }

    if (drop_servo.neutral() != 0) {

        printf("[PwmManager::initPwm] Failed to set drop servo to neutral\n");
        return false;
    }

    // すべてのモーターを停止状態に設定
    if (right_motor.stop() != 0) {

        printf("[PwmManager::initPwm] Failed to stop right motor\n");
        return false;
    }

    if (left_motor.stop() != 0) {

        printf("[PwmManager::initPwm] Failed to stop left motor\n");
        return false;
    }

    printf("[PwmManager::initPwm] All PWM controllers initialized successfully\n");
    return true;
}

bool PwmManager::setServoAngle(float angle[4]) {

    // angle[0] = エレベーター角度 [-90 ~ 90 deg]
    // angle[1] = ラダー角度 [-90 ~ 90 deg]
    // angle[2] = エルロン角度（右） [-90 ~ 90 deg]
    // angle[3] = 投下装置角度 [-90 ~ 90 deg]
    // 左エルロンは右エルロンの反対の角度

    uint8_t result = 0;

    result |= elevator_servo.setAngle(angle[0]);
    result |= rudder_servo.setAngle(angle[1]);
    result |= right_aileron_servo.setAngle(angle[2]);
    result |= left_aileron_servo.setAngle(angle[2]);  
    result |= drop_servo.setAngle(angle[3]);

    if (result != 0) {

        printf("[PwmManager::setServoAngle] Servo angle setting failed\n");
        return false;
    }

    return true;
}

bool PwmManager::setServoAngle_Independent(float angle[4]) {

    // テスト用：各サーボを独立して制御（左右エルロンも独立）
    // angle[0] = エレベーター角度 [-90 ~ 90 deg]
    // angle[1] = ラダー角度 [-90 ~ 90 deg]
    // angle[2] = エルロン右角度 [-90 ~ 90 deg]
    // angle[3] = エルロン左角度 [-90 ~ 90 deg]

    uint8_t result = 0;

    result |= elevator_servo.setAngle(angle[0]);
    result |= rudder_servo.setAngle(angle[1]);
    result |= right_aileron_servo.setAngle(angle[2]);
    result |= left_aileron_servo.setAngle(angle[3]);  // 独立制御（反転なし）

    if (result != 0) {

        printf("[PwmManager::setServoAngle_Independent] Servo angle setting failed\n");
        return false;
    }

    return true;
}

bool PwmManager::setMotorSpeed(float speed[2]) {

    // speed[0] = 右モータースピード [-100 ~ 100%]（リバースモード対応）
    // speed[1] = 左モータースピード [-100 ~ 100%]（リバースモード対応）

    uint8_t result = 0;

    result |= right_motor.setSpeed(speed[0]);
    result |= left_motor.setSpeed(speed[1]);

    if (result != 0) {

        printf("[PwmManager::setMotorSpeed] Motor speed setting failed\n");
        return false;
    }

    return true;
}
