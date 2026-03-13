#pragma once

#include <cstdint>
#include "../../Lib/1DoF_PID/PID.h"
#include "cascade_pid_config.hpp"

class CascadePIDManager {
public:
    explicit CascadePIDManager(float dt);

    // Cascade PID calculation for all three axes (pitch, roll, yaw)
    // Outputs [0]=pitch, [1]=roll, [2]=yaw servo commands
    // The rate inner loop runs every 2 cycles (controlled internally)
    // measured rates are currently 0.0f (TODO: update when gyro is connected)
    void calcCascadePIDAllAxes(
        float target_pitch, float measured_pitch,
        float target_roll, float measured_roll,
        float target_yaw, float measured_yaw,
        float pid_result[3],
        float measured_pitch_rate = 0.0f,
        float measured_roll_rate = 0.0f,
        float measured_yaw_rate = 0.0f
    );

    // Angle outer loop only (for AutoFlightState)
    float calcAnglePitch(float target_angle, float measured_angle);
    float calcAngleRoll(float target_angle, float measured_angle);
    float calcAngleYaw(float target_angle, float measured_angle);

    void reset();

private:
    PID angle_pitch_pid_;
    PID rate_pitch_pid_;
    PID angle_roll_pid_;
    PID rate_roll_pid_;
    PID angle_yaw_pid_;
    PID rate_yaw_pid_;

    uint32_t cycle_counter_ = 0;  // Internal counter for rate loop frequency control

    // Cascade PID calculation for each axis (private)
    float calcPitch(float target_angle, float measured_angle, float measured_rate = 0.0f);
    float calcRoll(float target_angle, float measured_angle, float measured_rate = 0.0f);
    float calcYaw(float target_angle, float measured_angle, float measured_rate = 0.0f);
};
