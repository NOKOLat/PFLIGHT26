#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


ProcessStatus LevelTurnState::onUpdate(StateContext& context) {



    // レベルターン用の更新処理
    return ProcessStatus::SUCCESS;
}


StateID LevelTurnState::evaluateNextState(StateContext& context) {

    // 安全スティックの値を確認（飛行終了判定）
    if(!context.rescaled_sbus_data.safety){

        return StateID::POST_FLIGHT_STATE;
    }

    // 手動飛行の判定
	if(context.rescaled_sbus_data.autofly == 0){

	    return StateID::MANUAL_FLIGHT_STATE;
	}

	return StateID::LEVEL_TURN_STATE;
}


StateID LevelTurnState::getStateID() const {

    return StateID::LEVEL_TURN_STATE;
}
