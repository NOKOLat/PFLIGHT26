#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


void PostFlightState::onEnter(StateContext& context) {

    // 飛行後状態固有の初期化処理
}


void PostFlightState::onExit(StateContext& context) {

    // 飛行後状態固有のクリーンアップ処理
}


StateResult PostFlightState::onUpdate(StateContext& context) {

    // 飛行後の処理

    StateResult result;
    result.success = true;
    result.should_transition = false;
    result.next_state_id = StateID::POST_FLIGHT_STATE;

    return result;
}


StateID PostFlightState::getStateID() const {

    return StateID::POST_FLIGHT_STATE;
}
