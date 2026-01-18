#include "States/AutoFlightStateBase/auto_landing_state.hpp"
#include "StateContext/context.hpp"


void AutoLandingState::onEnter(StateContext& context) {

    // 自動着陸状態固有の初期化処理
}


void AutoLandingState::onExit(StateContext& context) {

    // 自動着陸状態固有のクリーンアップ処理
}


StateResult AutoLandingState::onUpdate(StateContext& context) {

    // 自動着陸用の更新処理

    StateResult result;
    result.success = true;
    result.should_transition = true;
    result.next_state_id = StateID::POST_FLIGHT_STATE;

    return result;
}


StateID AutoLandingState::getStateID() const {

    return StateID::AUTO_LANDING_STATE;
}
