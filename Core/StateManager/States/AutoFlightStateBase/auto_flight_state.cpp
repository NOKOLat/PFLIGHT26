#include "States/AutoFlightStateBase/auto_flight_state.hpp"
#include "StateContext/context.hpp"


void AutoFlightState::onEnter(StateContext& context) {

    // 自動飛行状態固有の初期化処理
}


void AutoFlightState::onExit(StateContext& context) {

    // 自動飛行状態固有のクリーンアップ処理
}


StateResult AutoFlightState::onUpdate(StateContext& context) {

    // 自動飛行用の更新処理

    StateResult result;
    result.success = true;
    result.should_transition = false;
    result.next_state_id = StateID::AUTO_FLIGHT_STATE;

    return result;
}


StateID AutoFlightState::getStateID() const {

    return StateID::AUTO_FLIGHT_STATE;
}
