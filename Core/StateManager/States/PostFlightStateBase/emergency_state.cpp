#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


void EmergencyState::onEnter(StateContext& context) {

    // 緊急状態固有の初期化処理
    // モーターの即座停止
    // context.motor_driver->emergencyStop();

	while(1){

		printf("All Program Stop\n");
		HAL_Delay(10000);
	}
}


void EmergencyState::onExit(StateContext& context) {

    // 緊急状態固有のクリーンアップ処理
}


StateResult EmergencyState::onUpdate(StateContext& context) {

    // 緊急対応の処理
    return {true, false, StateID::EMERGENCY_STATE};
}


StateID EmergencyState::evaluateNextState(StateContext& context) {

    // 緊急状態では遷移しない
    return StateID::EMERGENCY_STATE;
}


StateID EmergencyState::getStateID() const {

    return StateID::EMERGENCY_STATE;
}
