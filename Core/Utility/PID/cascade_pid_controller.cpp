#include "cascade_pid_controller.hpp"
#include "../../Config/pid_config.hpp"
#include <stdio.h>

CascadePIDController::CascadePIDController() {
    // PIDインスタンスはメンバ変数として自動初期化される
}

bool CascadePIDController::initialize(float dt) {
    // Pitch軸
    angle_pid[PITCH].setGain(PidConfig::Pitch::Angle::KP, PidConfig::Pitch::Angle::KI, PidConfig::Pitch::Angle::KD);
    angle_pid[PITCH].setTime(dt);
    rate_pid[PITCH].setGain(PidConfig::Pitch::Rate::KP, PidConfig::Pitch::Rate::KI, PidConfig::Pitch::Rate::KD);
    rate_pid[PITCH].setTime(dt);

    // Roll軸
    angle_pid[ROLL].setGain(PidConfig::Roll::Angle::KP, PidConfig::Roll::Angle::KI, PidConfig::Roll::Angle::KD);
    angle_pid[ROLL].setTime(dt);
    rate_pid[ROLL].setGain(PidConfig::Roll::Rate::KP, PidConfig::Roll::Rate::KI, PidConfig::Roll::Rate::KD);
    rate_pid[ROLL].setTime(dt);

    // Yaw軸
    angle_pid[YAW].setGain(PidConfig::Yaw::Angle::KP, PidConfig::Yaw::Angle::KI, PidConfig::Yaw::Angle::KD);
    angle_pid[YAW].setTime(dt);
    rate_pid[YAW].setGain(PidConfig::Yaw::Rate::KP, PidConfig::Yaw::Rate::KI, PidConfig::Yaw::Rate::KD);
    rate_pid[YAW].setTime(dt);

    is_initialized = true;
    printf("[CascadePIDController::initialize] Successfully initialized with dt=%f\n", dt);
    return true;
}

float CascadePIDController::computeOuterLoop(int axis_index, float target_angle, float current_angle) {
    if (!is_initialized || axis_index < 0 || axis_index >= NUM_AXES) {
        printf("[CascadePIDController::computeOuterLoop] Invalid state or axis_index\n");
        return 0.0f;
    }

    angle_pid[axis_index].calc(target_angle, current_angle);
    return angle_pid[axis_index].getData();
}

float CascadePIDController::computeInnerLoop(int axis_index, float target_rate, float current_rate) {
    if (!is_initialized || axis_index < 0 || axis_index >= NUM_AXES) {
        printf("[CascadePIDController::computeInnerLoop] Invalid state or axis_index\n");
        return 0.0f;
    }

    rate_pid[axis_index].calc(target_rate, current_rate);
    return rate_pid[axis_index].getData();
}

void CascadePIDController::reset() {
    if (!is_initialized) return;
    for (int i = 0; i < NUM_AXES; ++i) {
        angle_pid[i].reset();
        rate_pid[i].reset();
    }
}

void CascadePIDController::resetAxis(int axis_index) {
    if (!is_initialized || axis_index < 0 || axis_index >= NUM_AXES) return;
    angle_pid[axis_index].reset();
    rate_pid[axis_index].reset();
}

void CascadePIDController::setOuterGain(int axis_index, float kp, float ki, float kd) {
    if (!is_initialized || axis_index < 0 || axis_index >= NUM_AXES) return;
    angle_pid[axis_index].setGain(kp, ki, kd);
}

void CascadePIDController::setInnerGain(int axis_index, float kp, float ki, float kd) {
    if (!is_initialized || axis_index < 0 || axis_index >= NUM_AXES) return;
    rate_pid[axis_index].setGain(kp, ki, kd);
}
