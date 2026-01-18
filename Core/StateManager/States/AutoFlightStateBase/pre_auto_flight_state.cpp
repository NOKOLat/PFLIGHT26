#include "States/AutoFlightStateBase/pre_auto_flight_state.hpp"
#include "StateContext/context.hpp"


void PreAutoFlightState::onEnter(StateContext& context) {

    // 自動飛行前準備状態固有の初期化処理
}


void PreAutoFlightState::onExit(StateContext& context) {

    // 自動飛行前準備状態固有のクリーンアップ処理
}


StateResult PreAutoFlightState::onUpdate(StateContext& context) {

    // 自動飛行前準備用の更新処理

    StateResult result;
    result.success = true;
    result.should_transition = true;
    result.next_state_id = StateID::AUTO_TAKEOFF_STATE;

    return result;
}


StateID PreAutoFlightState::getStateID() const {

    return StateID::PRE_AUTO_FLIGHT_STATE;
}
