#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


void AutoFlightState::onEnter(StateContext& context) {

    // 自動飛行状態固有の初期化処理
}


void AutoFlightState::onExit(StateContext& context) {

    // 自動飛行状態固有のクリーンアップ処理
}


StateResult AutoFlightState::onUpdate(StateContext& context) {

    // 自動飛行用の更新処理
    return {true, false, StateID::AUTO_FLIGHT_STATE};
}


StateID AutoFlightState::evaluateNextState(StateContext& context) {

    // AutoFlightState から他の状態への遷移判定をここに実装
    // デフォルトは現在の状態を継続
    return StateID::AUTO_FLIGHT_STATE;
}


StateID AutoFlightState::getStateID() const {

    return StateID::AUTO_FLIGHT_STATE;
}
