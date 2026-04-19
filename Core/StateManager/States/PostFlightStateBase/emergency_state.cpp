#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"

ProcessStatus EmergencyState::onUpdate(StateContext& context) {

    // モーターを停止
    float motor_stop[2] = {0.0f, 0.0f};
    if (context.instances.pwm_controller) {
        context.instances.pwm_controller->setMotorSpeed(motor_stop);
    }

    return ProcessStatus::SUCCESS;
}


StateID EmergencyState::evaluateNextState(StateContext& context) {

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
	printf("error state stop\n");

	while(1){

	    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_4);

		HAL_Delay(1000);
	}


    // 緊急状態では遷移しない
    return StateID::EMERGENCY_STATE;
}


StateID EmergencyState::getStateID() const {

    return StateID::EMERGENCY_STATE;
}
