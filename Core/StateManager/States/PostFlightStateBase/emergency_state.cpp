#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"

StateResult EmergencyState::onUpdate(StateContext& context) {

    // 緊急対応の処理
    return {true, false, StateID::EMERGENCY_STATE};
}


StateID EmergencyState::evaluateNextState(StateContext& context) {

    // 緊急状態では遷移しない
    return StateID::EMERGENCY_STATE;
}


StateID EmergencyState::getStateID() const {

    return StateID::EMERGENCY_STATE;
}
