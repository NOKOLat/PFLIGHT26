#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


ProcessStatus LevelFlightState::onUpdate(StateContext& context) {

    // 水平飛行用の更新処理
    return ProcessStatus::SUCCESS;
}


StateID LevelFlightState::evaluateNextState(StateContext& context) {

    // 安全スティックの値を確認（飛行終了判定）
    if(!context.rescaled_sbus_data.safety){

        return StateID::POST_FLIGHT_STATE;
    }

    // 手動飛行の判定
    if(context.rescaled_sbus_data.autofly == 0){

        return StateID::MANUAL_FLIGHT_STATE;
    }

	if(context.rescaled_sbus_data.autofly == 2){

		// 水平旋回
		printf("switch %d\n", context.rescaled_sbus_data.selectmission);
		if(context.rescaled_sbus_data.selectmission == 0){

			return StateID::LEVEL_TURN_STATE;
		}

		if(context.rescaled_sbus_data.selectmission == 1){

			return StateID::CLIMBING_TURN_STATE;
		}

		if(context.rescaled_sbus_data.selectmission == 2){

			return StateID::FUGUE_EIGHT_STATE;
		}
	}

    return StateID::LEVEL_FLIGHT_STATE;
}


StateID LevelFlightState::getStateID() const {

    return StateID::LEVEL_FLIGHT_STATE;
}
