#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


ProcessStatus ClimbingTurnState::onUpdate(StateContext& context) {

    // クライミングターン用の更新処理
    return ProcessStatus::SUCCESS;
}


StateID ClimbingTurnState::evaluateNextState(StateContext& context) {

    // 安全スティックの値を確認（飛行終了判定）
    if(!context.rescaled_sbus_data.safety){

        return StateID::POST_FLIGHT_STATE;
    }

    // 手動飛行の判定
	if(context.rescaled_sbus_data.autofly == 0){

	    return StateID::MANUAL_FLIGHT_STATE;
	}

	return StateID::CLIMBING_TURN_STATE;
}


StateID ClimbingTurnState::getStateID() const {

    return StateID::CLIMBING_TURN_STATE;
}
