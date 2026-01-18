#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


void PreFlightState::enter(StateContext& context) {

    // 飛行前の準備処理の初期化
}


StateResult PreFlightState::update(StateContext& context) {

    // 飛行前の準備処理

    StateResult result;
    result.success = true;
    result.should_transition = true;
    result.next_state_id = StateID::MANUAL_FLIGHT_STATE;

    return result;
}


void PreFlightState::exit(StateContext& context) {

    // クリーンアップ処理
}


StateID PreFlightState::getStateID() const {

    return StateID::PRE_FLIGHT_STATE;
}


StateBaseID PreFlightState::getStateBaseID() const {

    return StateBaseID::PRE_FLIGHT_STATE_BASE;
}
