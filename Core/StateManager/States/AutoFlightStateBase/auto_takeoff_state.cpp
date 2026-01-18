#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


void AutoTakeoffState::onEnter(StateContext& context) {

    // 自動離陸状態固有の初期化処理
}


void AutoTakeoffState::onExit(StateContext& context) {

    // 自動離陸状態固有のクリーンアップ処理
}


StateResult AutoTakeoffState::onUpdate(StateContext& context) {

    // 自動離陸用の更新処理

    StateResult result;
    result.success = true;
    result.should_transition = true;
    result.next_state_id = StateID::AUTO_FLIGHT_STATE;

    return result;
}


StateID AutoTakeoffState::getStateID() const {

    return StateID::AUTO_TAKEOFF_STATE;
}
