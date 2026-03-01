#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"

ProcessStatus EmergencyState::onUpdate(StateContext& context) {

    // 緊急対応の処理
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
