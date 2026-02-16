#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


ProcessStatus PreAutoFlightState::onUpdate(StateContext& context) {

    return ProcessStatus::SUCCESS;
}


StateID PreAutoFlightState::evaluateNextState(StateContext& context) {

    // PreAutoFlightState から他の状態への遷移判定をここに実装
    // デフォルトは現在の状態を継続
    return StateID::PRE_AUTO_FLIGHT_STATE;
}


StateID PreAutoFlightState::getStateID() const {

    return StateID::PRE_AUTO_FLIGHT_STATE;
}
