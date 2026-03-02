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

    // 姿勢推定結果を AttitudeState に格納
    context.attitude_state.angle.roll()  = AttitudeEKF_GetRoll(&context.instances.attitude_ekf.value())  * context.unit_conversion.RAD_TO_DEG;
    context.attitude_state.angle.pitch() = AttitudeEKF_GetPitch(&context.instances.attitude_ekf.value()) * context.unit_conversion.RAD_TO_DEG;
    context.attitude_state.angle.yaw()   = AttitudeEKF_GetYaw(&context.instances.attitude_ekf.value())   * context.unit_conversion.RAD_TO_DEG;

    // 高度推定の更新
    if (context.instances.altitude_estimator.has_value()) {

        // 角度をVector3fに変換して高度推定に渡す
        Vector3f angle_vec;
        angle_vec.setX(context.attitude_state.angle.roll());
        angle_vec.setY(context.attitude_state.angle.pitch());
        angle_vec.setZ(context.attitude_state.angle.yaw());

        context.instances.altitude_estimator->Update(
            context.sensor_data.barometric_pressure,
            context.sensor_data.accel.getptr(),
            angle_vec.getptr(),
            context.loop_time_us / 1000000.0f
        );

        float altitude_data[3];
        context.instances.altitude_estimator->GetData(altitude_data);

        // 高度推定結果を AttitudeState に格納
        context.attitude_state.altitude = altitude_data[0];
    }

    // 5ループに1回センサーデータと角度データを出力
    if(loop_count % 5 == 0) {

        printf("Angle: %.2f, %.2f, %.2f | Alt: %.3f m\n",
            context.attitude_state.angle.roll(),
            context.attitude_state.angle.pitch(),
            context.attitude_state.angle.yaw(),
            context.attitude_state.altitude);
    }

    return ProcessStatus::SUCCESS;
}


StateID SensorTestState::evaluateNextState(StateContext& context) {

    // プリフライトデバッグが0になったらPRE_FLIGHT_STATEに戻る
    if(context.rescaled_sbus_data.preflight_debug == SwitchPosition::LOW){

        return StateID::PRE_FLIGHT_STATE;
    }

    // プリフライトデバッグが HIGH になったらSERVO_TEST_STATEに遷移
    if(context.rescaled_sbus_data.preflight_debug == SwitchPosition::HIGH){

        return StateID::SERVO_TEST_STATE;
    }

    // 遷移せず現在の状態を継続
    return StateID::SENSOR_TEST_STATE;
}


StateID SensorTestState::getStateID() const {

    return StateID::SENSOR_TEST_STATE;
}
