#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


ProcessStatus AutoTakeoffState::onUpdate(StateContext& context) {

    // 自動離陸用の更新処理
    return ProcessStatus::SUCCESS;
}


StateID AutoTakeoffState::evaluateNextState(StateContext& context) {

    // 自動離陸完了後はAUTO_FLIGHT_STATEに遷移
    return StateID::AUTO_FLIGHT_STATE;
}


StateID AutoTakeoffState::getStateID() const {

    return StateID::AUTO_TAKEOFF_STATE;
}
