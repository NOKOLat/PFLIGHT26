#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


ProcessStatus SensorTestState::onUpdate(StateContext& context) {

    loop_count++;

    // センサーデータの取得
    context.instances.sensor_manager->updateSensors();
    context.instances.sensor_manager->getAccelData(&context.sensor_data.accel);
    context.instances.sensor_manager->getGyroData(&context.sensor_data.gyro);
    context.instances.sensor_manager->getMagData(&context.sensor_data.mag);

    // 姿勢推定 (EKF)
    {
        const float_prec accel[3] = {
            context.sensor_data.accel[Axis::X],
            context.sensor_data.accel[Axis::Y],
            context.sensor_data.accel[Axis::Z]
        };
        // ジャイロをdeg/s -> rad/sに変換
        const float_prec gyro[3] = {
            context.sensor_data.gyro[Axis::X] * StateContext::DEG_TO_RAD,
            context.sensor_data.gyro[Axis::Y] * StateContext::DEG_TO_RAD,
            context.sensor_data.gyro[Axis::Z] * StateContext::DEG_TO_RAD
        };
        AttitudeEKF_Update(&context.instances.attitude_ekf.value(), accel, gyro);
    }
    context.sensor_data.angle[Axis::X] = AttitudeEKF_GetRoll(&context.instances.attitude_ekf.value())  * StateContext::RAD_TO_DEG;
    context.sensor_data.angle[Axis::Y] = AttitudeEKF_GetPitch(&context.instances.attitude_ekf.value()) * StateContext::RAD_TO_DEG;
    context.sensor_data.angle[Axis::Z] = AttitudeEKF_GetYaw(&context.instances.attitude_ekf.value())   * StateContext::RAD_TO_DEG;

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

        printf("Angle: %f, %f, %f\n",
            context.sensor_data.angle[Axis::X],
            context.sensor_data.angle[Axis::Y],
            context.sensor_data.angle[Axis::Z]);
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
