#include "StateBase/state_base.hpp"
#include "StateContext/context.hpp"


void ManualFlightStateBase::enter(StateContext& context) {

    // 共通の初期化処理

    // 派生クラス固有の初期化処理を呼び出す
    onEnter(context);
}


StateResult ManualFlightStateBase::update(StateContext& context) {

    // 共通の更新処理

	// 0. 安全装置のチェック
	if(!context.rescaled_sbus_data.aux5){

		printf("End ManualFlight SBUS[9] = %d\n", context.rescaled_sbus_data.aux5);
		return {true, true, StateID::POST_FLIGHT_STATE};
	}

	if(context.rescaled_sbus_data.aux5 && StateID::MANUAL_FLIGHT_STATE){


	}



    // debug sbusdataのチェック
	// context.rescaled_sbus_data.throttle = 20.0f;
	// context.rescaled_sbus_data.elevator = 35.0f;
	// context.rescaled_sbus_data.rudder   = 10.0f;
	// context.rescaled_sbus_data.aileron  = 5.0f;


    // 1. センサーデータの取得
	context.instances.imu_sensor->GetData(context.sensor_data.accel.getptr(), context.sensor_data.gyro.getptr());
	context.instances.baro_sensor->getData(&context.sensor_data.barometric_pressure, &context.sensor_data.temperature);
	context.instances.mag_sensor->getdata(context.sensor_data.mag.getptr());

	// 2. 姿勢推定
	context.instances.madgwick->updateIMU(context.sensor_data.gyro[Axis::X], context.sensor_data.gyro[Axis::Y], context.sensor_data.gyro[Axis::Z], context.sensor_data.accel[Axis::X], context.sensor_data.accel[Axis::Y], context.sensor_data.accel[Axis::Z]);

    context.sensor_data.angle[Axis::X] = context.instances.madgwick->getRoll();
    context.sensor_data.angle[Axis::Y] = context.instances.madgwick->getPitch();
    context.sensor_data.angle[Axis::Z] = context.instances.madgwick->getYaw();
    
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

	// debug センサーデータの確認
	//printf("Accel: %f, %f, %f\n", context.sensor_data.accel[Axis::X], context.sensor_data.accel[Axis::Y], context.sensor_data.accel[Axis::Z]);

    //debug 角度データの確認
    //printf("Angle: %f, %f %f\n", context.sensor_data.angle[Axis::X], context.sensor_data.angle[Axis::Y], context.sensor_data.angle[Axis::Z]);

    //debug PWMデータの確認
    printf("PWM: %f, %f, %f, %f\n", context.control_output.servo_pwm[0], context.control_output.servo_pwm[1], context.control_output.servo_pwm[2], context.control_output.servo_pwm[3]);

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
