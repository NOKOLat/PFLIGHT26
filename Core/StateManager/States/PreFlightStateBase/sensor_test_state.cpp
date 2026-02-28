#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


ProcessStatus SensorTestState::onUpdate(StateContext& context) {

    loop_count++;

    // センサーデータの取得
    context.instances.sensor_manager->updateSensors();
    context.instances.sensor_manager->getAccelData(&context.sensor_data.accel);
    context.instances.sensor_manager->getGyroData(&context.sensor_data.gyro);
    context.instances.sensor_manager->getMagData(&context.sensor_data.mag);
    context.instances.sensor_manager->getPressData(&context.sensor_data.barometric_pressure);

    // 姿勢推定 (EKF)
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

    // 高度推定の更新
    if (context.instances.altitude_estimator.has_value()) {

        context.instances.altitude_estimator->Update(
            context.sensor_data.barometric_pressure,
            context.sensor_data.accel.getptr(),
            context.sensor_data.angle.getptr(),
            context.loop_time_us / 1000000.0f
        );
        
        float altitude_data[3];
        context.instances.altitude_estimator->GetData(altitude_data);

        context.sensor_data.altitude          = altitude_data[0];
        context.sensor_data.altitude_velocity = altitude_data[1];
        context.sensor_data.altitude_accel    = altitude_data[2];
    }

    // 5ループに1回センサーデータと角度データを出力
    if(loop_count % 5 == 0) {
//        printf("Angle: %f, %f, %f| Accel: %f, %f, %f| gyro: %f, %f, %f | mag: %f, %f, %f\n",
//            context.sensor_data.angle[Axis::X],
//            context.sensor_data.angle[Axis::Y],
//            context.sensor_data.angle[Axis::Z],
//            context.sensor_data.accel[Axis::X],
//            context.sensor_data.accel[Axis::Y],
//            context.sensor_data.accel[Axis::Z],
//            context.sensor_data.gyro[Axis::X],
//            context.sensor_data.gyro[Axis::Y],
//            context.sensor_data.gyro[Axis::Z],
//            context.sensor_data.mag[Axis::X],
//            context.sensor_data.mag[Axis::Y],
//            context.sensor_data.mag[Axis::Z]);

        printf("Angle: %f, %f, %f | Alt: %.3f m, Vel: %.3f m/s\n",
            context.sensor_data.angle[Axis::X],
            context.sensor_data.angle[Axis::Y],
            context.sensor_data.angle[Axis::Z],
            context.sensor_data.altitude,
            context.sensor_data.altitude_velocity);
    }

    return ProcessStatus::SUCCESS;
}


StateID SensorTestState::evaluateNextState(StateContext& context) {

    // プリフライトデバッグが0になったらPRE_FLIGHT_STATEに戻る
    if(context.rescaled_sbus_data.preflight_debug == 0){

        return StateID::PRE_FLIGHT_STATE;
    }

    // プリフライトデバッグが2になったらSERVO_TEST_STATEに遷移
    if(context.rescaled_sbus_data.preflight_debug == 2){

        return StateID::SERVO_TEST_STATE;
    }

    // 遷移せず現在の状態を継続
    return StateID::SENSOR_TEST_STATE;
}


StateID SensorTestState::getStateID() const {

    return StateID::SENSOR_TEST_STATE;
}
