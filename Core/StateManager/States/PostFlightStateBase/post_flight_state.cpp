#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


ProcessStatus PostFlightState::onUpdate(StateContext& context) {

    // モーターを停止
    float motor_stop[2] = {0.0f, 0.0f};
    if (context.instances.pwm_controller) {
        context.instances.pwm_controller->setMotorSpeed(motor_stop);
    }

    // 飛行中に蓄積したデータをリセット
    if (context.instances.sensor_fusion_manager.has_value()) {
        SensorManager* sensor_mgr = context.instances.sensor_fusion_manager->getSensorManager();
        if (sensor_mgr != nullptr) {
            sensor_mgr->resetMovingAverages();
        }
    }

    // GPIO設定
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);

    // 飛行後の処理
    HAL_Delay(1000);

    return ProcessStatus::SUCCESS;
}


StateID PostFlightState::evaluateNextState(StateContext& context) {

    // 飛行後の処理完了後はPRE_FLIGHT_STATEに遷移
    return StateID::PRE_FLIGHT_STATE;
}


StateID PostFlightState::getStateID() const {

    return StateID::POST_FLIGHT_STATE;
}
