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
    // ジャイロをdeg/s -> rad/sに変換
	const float_prec gyro[3] = {
		context.sensor_data.gyro[Axis::X] * context.unit_conversion.DEG_TO_RAD,
		context.sensor_data.gyro[Axis::Y] * context.unit_conversion.DEG_TO_RAD,
		context.sensor_data.gyro[Axis::Z] * context.unit_conversion.DEG_TO_RAD
	};

	AttitudeEKF_Update(&context.instances.attitude_ekf.value(), context.sensor_data.accel.getptr(), gyro);

    context.sensor_data.angle[Axis::X] = AttitudeEKF_GetRoll(&context.instances.attitude_ekf.value())  * context.unit_conversion.RAD_TO_DEG;
    context.sensor_data.angle[Axis::Y] = AttitudeEKF_GetPitch(&context.instances.attitude_ekf.value()) * context.unit_conversion.RAD_TO_DEG;
    context.sensor_data.angle[Axis::Z] = AttitudeEKF_GetYaw(&context.instances.attitude_ekf.value())   * context.unit_conversion.RAD_TO_DEG;

    // 2-5. 高度推定の更新
    context.instances.altitude_estimator->Update(
        context.sensor_data.barometric_pressure,
        context.sensor_data.accel.getptr(),
        context.sensor_data.angle.getptr(),
        context.loop_time_us / 1000000.0f  // μs to seconds
    );

    // 高度推定結果を取得
    float altitude_data[3];  // [altitude, velocity, accel]
    context.instances.altitude_estimator->GetData(altitude_data);
    context.sensor_data.altitude          = altitude_data[0];
    context.sensor_data.altitude_velocity = altitude_data[1];
    context.sensor_data.altitude_accel    = altitude_data[2];

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
