#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


ProcessStatus FugueEightState::onUpdate(StateContext& context) {

    // フュージュエイト用の更新処理
    return ProcessStatus::SUCCESS;
}


StateID FugueEightState::evaluateNextState(StateContext& context) {

    // 安全スティックの値を確認（飛行終了判定）
    if(!context.rescaled_sbus_data.safety){

        return StateID::POST_FLIGHT_STATE;
    }

    // 手動飛行の判定
	if(context.rescaled_sbus_data.autofly == 0){

	    return StateID::MANUAL_FLIGHT_STATE;
	}

	return StateID::FUGUE_EIGHT_STATE;
}


StateID FugueEightState::getStateID() const {

    return StateID::FUGUE_EIGHT_STATE;
}
