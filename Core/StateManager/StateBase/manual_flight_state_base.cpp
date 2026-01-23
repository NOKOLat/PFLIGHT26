#include "StateBase/state_base.hpp"
#include "StateContext/context.hpp"


void ManualFlightStateBase::enter(StateContext& context) {

    // 共通の初期化処理

    // 派生クラス固有の初期化処理を呼び出す
    onEnter(context);
}


StateResult ManualFlightStateBase::update(StateContext& context) {

    // 共通の更新処理

    // 0. SBUSデータの取得
    // StateManager側で更新

    // 1. センサーデータの取得
	context.instances.imu_sensor->GetData(context.sensor_data.accel.getptr(), context.sensor_data.gyro.getptr());
	context.instances.baro_sensor->getData(&context.sensor_data.barometric_pressure, &context.sensor_data.temperature);
	context.instances.mag_sensor->getdata(context.sensor_data.mag.getptr());

	// 2-1. EKF 予測ステップ
	context.instances.ekf->prediction();

	// 2-2. EKF 更新ステップ（センサーデータを使用）
	//　ジャイロ: rad / s、加速度: 正規化済み、磁気センサ: 正規化済み
	context.instances.ekf->update_accel(context.sensor_data.accel[Axis::X], context.sensor_data.accel[Axis::Y], context.sensor_data.accel[Axis::Z]);
	context.instances.ekf->update_gyro(context.sensor_data.gyro[Axis::X], context.sensor_data.gyro[Axis::Y], context.sensor_data.gyro[Axis::Z]);
	context.instances.ekf->update_mag(context.sensor_data.mag[Axis::X], -context.sensor_data.mag[Axis::Y], context.sensor_data.mag[Axis::Z]);

	// 2-3. 推定状態からオイラー角を計算
	context.instances.ekf->euler_angles();

	// 2-4. オイラー角を度単位で取得
	context.sensor_data.angle[Axis::X] = context.instances.ekf->Roll * (180.0f / M_PI) -180;   // Roll
	context.sensor_data.angle[Axis::Y] = context.instances.ekf->Pitch * (180.0f / M_PI);  // Pitch
	context.sensor_data.angle[Axis::Z] = context.instances.ekf->Yaw * (180.0f / M_PI) + 180;    // Yaw

    // 3. 派生クラス固有の更新処理を呼び出す（PID計算）
    StateResult result = onUpdate(context);

    // 4. モーターのPWM出力
    context.instances.right_motor->setSpeed(context.control_output.motor_pwm[0]);
    context.instances.left_motor->setSpeed(context.control_output.motor_pwm[1]);

    // 5. サーボのPWM出力
    context.instances.elevator_servo->setAngle(context.control_output.servo_pwm[0]);
    context.instances.rudder_servo->setAngle(context.control_output.servo_pwm[1]);
    context.instances.aileron_servo->setAngle(context.control_output.servo_pwm[2]);
    context.instances.aileron_servo->setAngle(context.control_output.servo_pwm[3]);

    //debug code
    printf("Angle: %f, %f %f\n", context.sensor_data.angle[Axis::X], context.sensor_data.angle[Axis::Y], context.sensor_data.angle[Axis::Z]);

    return result;
}


void ManualFlightStateBase::exit(StateContext& context) {

    // 派生クラス固有のクリーンアップ処理を呼び出す
    onExit(context);

    // 共通のクリーンアップ処理
}


StateBaseID ManualFlightStateBase::getStateBaseID() const {

    return StateBaseID::MANUAL_FLIGHT_STATE_BASE;
}
