#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


void PreAutoFlightState::onEnter(StateContext& context) {

    // 自動飛行前準備状態固有の初期化処理
}


void PreAutoFlightState::onExit(StateContext& context) {

    // 自動飛行前準備状態固有のクリーンアップ処理
}


StateResult PreAutoFlightState::onUpdate(StateContext& context) {

    return {true, false, StateID::PRE_AUTO_FLIGHT_STATE};
}


StateID PreAutoFlightState::evaluateNextState(StateContext& context) {

    // PreAutoFlightState から他の状態への遷移判定をここに実装
    // デフォルトは現在の状態を継続
    return StateID::PRE_AUTO_FLIGHT_STATE;
}


StateID PreAutoFlightState::getStateID() const {

    return StateID::PRE_AUTO_FLIGHT_STATE;
}
