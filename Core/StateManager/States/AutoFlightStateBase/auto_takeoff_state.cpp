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
    return {true, false, StateID::AUTO_TAKEOFF_STATE};
}


StateID AutoTakeoffState::evaluateNextState(StateContext& context) {

    // 自動離陸完了後はAUTO_FLIGHT_STATEに遷移
    return StateID::AUTO_FLIGHT_STATE;
}


StateID AutoTakeoffState::getStateID() const {

    return StateID::AUTO_TAKEOFF_STATE;
}
