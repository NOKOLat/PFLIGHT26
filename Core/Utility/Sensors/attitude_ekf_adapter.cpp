#include "attitude_ekf_adapter.hpp"

AttitudeEkfAdapter::AttitudeEkfAdapter() : ekf_sys_{} {
}

bool AttitudeEkfAdapter::init(float dt) {
    return AttitudeEKF_Init(&ekf_sys_, dt);
}

bool AttitudeEkfAdapter::reset() {
    return AttitudeEKF_Reset(&ekf_sys_);
}

bool AttitudeEkfAdapter::update(const float accel[3], const float gyro[3]) {
    return AttitudeEKF_Update(&ekf_sys_, accel, gyro);
}

void AttitudeEkfAdapter::getAttitudeRad(float angles[3]) const {
    AttitudeEKF_GetAngles(&ekf_sys_, angles);
}

float AttitudeEkfAdapter::getRollRad() const {
    return AttitudeEKF_GetRoll(&ekf_sys_);
}

float AttitudeEkfAdapter::getPitchRad() const {
    return AttitudeEKF_GetPitch(&ekf_sys_);
}

float AttitudeEkfAdapter::getYawRad() const {
    return AttitudeEKF_GetYaw(&ekf_sys_);
}
