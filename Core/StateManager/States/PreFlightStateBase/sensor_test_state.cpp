#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


ProcessStatus SensorTestState::onUpdate(StateContext& context) {

    loop_count++;

    // 1. 姿勢推定の更新（SensorTestState でのみ実行）
    float dt = context.loop_time_us / 1000000.0f;
    if (!context.attitude_estimation.update(dt)) {
        printf("[SensorTestState::onUpdate] Attitude Estimation Update Failed\n");
    }
    context.attitude_state = context.attitude_estimation.getAttitudeState();

    // 2. 5ループに1回センサーデータと角度データを出力
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
