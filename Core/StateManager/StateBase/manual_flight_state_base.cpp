#include "StateBase/state_base.hpp"
#include "StateContext/context.hpp"

StateResult ManualFlightStateBase::update(StateContext& context) {

    // 共通の更新処理

    // 0. 状態の判定（SBUSのデータを用いて判定）
    StateID next_state = evaluateNextState(context);

    // 状態遷移が必要な場合は、遷移フラグを立てて次の状態IDをセット
    if(next_state != getStateID()) {

        return {ProcessStatus::SUCCESS, TransitionFlag::SHOULD_TRANSITION, next_state};
    }

    // 1. センサーデータの取得
    context.instances.sensor_manager->updateSensors();
    context.instances.sensor_manager->getAccelData(&context.sensor_data.accel);
    context.instances.sensor_manager->getGyroData(&context.sensor_data.gyro);
    context.instances.sensor_manager->getMagData(&context.sensor_data.mag);
    context.instances.sensor_manager->getPressData(&context.sensor_data.barometric_pressure);
    context.instances.sensor_manager->getTempData(&context.sensor_data.temperature);

    // 2. 姿勢推定 (EKF)
    {
        const float_prec accel[3] = {
            context.sensor_data.accel[Axis::X],
            context.sensor_data.accel[Axis::Y],
            context.sensor_data.accel[Axis::Z]
        };
        const float_prec gyro[3] = {
            context.sensor_data.gyro[Axis::X],
            context.sensor_data.gyro[Axis::Y],
            context.sensor_data.gyro[Axis::Z]
        };
        AttitudeEKF_Update(&context.instances.attitude_ekf.value(), accel, gyro);
    }
    constexpr float RAD_TO_DEG = 180.0f / 3.14159265358979323846f;
    context.sensor_data.angle[Axis::X] = AttitudeEKF_GetRoll(&context.instances.attitude_ekf.value())  * RAD_TO_DEG;
    context.sensor_data.angle[Axis::Y] = AttitudeEKF_GetPitch(&context.instances.attitude_ekf.value()) * RAD_TO_DEG;
    context.sensor_data.angle[Axis::Z] = AttitudeEKF_GetYaw(&context.instances.attitude_ekf.value())   * RAD_TO_DEG;

    // 3. 派生クラス固有の更新処理を呼び出す（制御出力）
    ProcessStatus status = onUpdate(context);

    // 4. PWM出力（PwmManager経由）
    context.instances.pwm_controller->setMotorSpeed(context.control_output.motor_pwm.data());
    context.instances.pwm_controller->setServoAngle(context.control_output.servo_pwm.data());

	// debug センサーデータの確認
	//printf("Accel: %f, %f, %f\n", context.sensor_data.accel[Axis::X], context.sensor_data.accel[Axis::Y], context.sensor_data.accel[Axis::Z]);

    // debug 角度データの確認
    //printf("Angle: %f, %f %f\n", context.sensor_data.angle[Axis::X], context.sensor_data.angle[Axis::Y], context.sensor_data.angle[Axis::Z]);

    // debug PWMデータの確認
    //printf("PWM: %f, %f, %f, %f\n", context.control_output.servo_pwm[static_cast<int>(PwmConfig::ServoChannel::ELEVATOR)], context.control_output.servo_pwm[static_cast<int>(PwmConfig::ServoChannel::RUDDER)], context.control_output.servo_pwm[static_cast<int>(PwmConfig::ServoChannel::AILERON)], context.control_output.servo_pwm[static_cast<int>(PwmConfig::ServoChannel::DROP)]);

    return {status, TransitionFlag::NO_TRANSITION, getStateID()};
}


StateBaseID ManualFlightStateBase::getStateBaseID() const {

    return StateBaseID::MANUAL_FLIGHT_STATE_BASE;
}
