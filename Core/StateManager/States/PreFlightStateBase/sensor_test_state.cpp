#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


ProcessStatus SensorTestState::onUpdate(StateContext& context) {

    loop_count++;

    // センサー融合マネージャーが初期化されているか確認
    if (!context.instances.sensor_fusion_manager.has_value()) {

        printf("Error: SensorFusionManager is not initialized\n");
        return ProcessStatus::FAILURE;
    }

    // センサー融合を更新（センサー取得 + 姿勢推定 + 高度推定）
    if (!context.instances.sensor_fusion_manager->update()) {

        printf("Warning: SensorFusionManager update failed\n");
        return ProcessStatus::FAILURE;
    }

    // 推定結果を StateContext に保存
    context.attitude = context.instances.sensor_fusion_manager->getAttitude();
    context.altitude = context.instances.sensor_fusion_manager->getAltitude();

    // 5ループに1回センサーデータと角度データを出力
    if (loop_count % 5 == 0) {

        printf("Angle: %.2f, %.2f, %.2f | Alt: %.3f m\n",
            context.attitude.roll(),
            context.attitude.pitch(),
            context.attitude.yaw(),
            context.altitude);
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
