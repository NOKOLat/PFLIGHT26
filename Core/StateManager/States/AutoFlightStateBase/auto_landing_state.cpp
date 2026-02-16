#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


void AutoLandingState::onEnter(StateContext& context) {

    // 自動着陸状態固有の初期化処理
}


void AutoLandingState::onExit(StateContext& context) {

    // 自動着陸状態固有のクリーンアップ処理
}


StateResult AutoLandingState::onUpdate(StateContext& context) {

    // 自動着陸用の更新処理
    return {true, false, StateID::AUTO_LANDING_STATE};
}


StateID AutoLandingState::evaluateNextState(StateContext& context) {

    // 自動着陸完了後はPOST_FLIGHT_STATEに遷移
    return StateID::POST_FLIGHT_STATE;
}


StateID AutoLandingState::getStateID() const {

    return StateID::AUTO_LANDING_STATE;
}
