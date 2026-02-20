#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


ProcessStatus PreFlightState::onUpdate(StateContext& context) {

    loop_count++;
	if(loop_count % 10 == 0){

		printf("Channel[3]: %f SBUS[9] = %d\n", context.rescaled_sbus_data.throttle, context.rescaled_sbus_data.safety);
	}

    return ProcessStatus::SUCCESS;
}


StateID PreFlightState::evaluateNextState(StateContext& context) {

    // 安全スティックの値を確認
    if(context.rescaled_sbus_data.safety){

        return StateID::MANUAL_FLIGHT_STATE;
    }

    // 遷移せず現在の状態を継続
    return StateID::PRE_FLIGHT_STATE;
}


StateID PreFlightState::getStateID() const {

    return StateID::PRE_FLIGHT_STATE;
}
