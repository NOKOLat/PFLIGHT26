#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


ProcessStatus SensorTestState::onUpdate(StateContext& context) {

    loop_count++;

    // センサーデータの取得
    context.instances.sensor_manager->updateSensors();
    context.instances.sensor_manager->getAccelData(&context.sensor_data.accel);
    context.instances.sensor_manager->getGyroData(&context.sensor_data.gyro);
    context.instances.sensor_manager->getMagData(&context.sensor_data.mag);

    // 姿勢推定
    context.instances.madgwick->updateIMU(
        context.sensor_data.gyro[Axis::X],  context.sensor_data.gyro[Axis::Y],  context.sensor_data.gyro[Axis::Z],
        context.sensor_data.accel[Axis::X], context.sensor_data.accel[Axis::Y], context.sensor_data.accel[Axis::Z]);

    context.sensor_data.angle[Axis::X] = context.instances.madgwick->getRoll();
    context.sensor_data.angle[Axis::Y] = context.instances.madgwick->getPitch();
    context.sensor_data.angle[Axis::Z] = context.instances.madgwick->getYaw();

    // 5ループに1回センサーデータと角度データを出力
    if(loop_count % 5 == 0) {
        printf("Angle: %f, %f, %f| Accel: %f, %f, %f| gyro: %f, %f, %f | mag: %f, %f, %f\n",
            context.sensor_data.angle[Axis::X],
            context.sensor_data.angle[Axis::Y],
            context.sensor_data.angle[Axis::Z],
            context.sensor_data.accel[Axis::X],
            context.sensor_data.accel[Axis::Y],
            context.sensor_data.accel[Axis::Z],
            context.sensor_data.gyro[Axis::X],
            context.sensor_data.gyro[Axis::Y],
            context.sensor_data.gyro[Axis::Z],
            context.sensor_data.mag[Axis::X],
            context.sensor_data.mag[Axis::Y],
            context.sensor_data.mag[Axis::Z]);
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
