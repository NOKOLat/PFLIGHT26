#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


ProcessStatus ServoMotorTestState::onUpdate(StateContext& context) {

    return ProcessStatus::SUCCESS;
}


StateID ServoMotorTestState::evaluateNextState(StateContext& context) {

    // プリフライトデバッグが0になったらPRE_FLIGHT_STATEに戻る
    if(context.rescaled_sbus_data.preflight_debug == 0){

        return StateID::PRE_FLIGHT_STATE;
    }

    // 遷移せず現在の状態を継続
    return StateID::SERVO_TEST_STATE;
}


StateID ServoMotorTestState::getStateID() const {

    return StateID::SERVO_TEST_STATE;
}
