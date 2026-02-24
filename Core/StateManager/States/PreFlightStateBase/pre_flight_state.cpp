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

    // 手動飛行モード
    if(context.rescaled_sbus_data.safety && context.rescaled_sbus_data.preflight_debug == 0 && context.rescaled_sbus_data.autofly == 0){

        return StateID::MANUAL_FLIGHT_STATE;
    }

    // センサーテスト
    if(context.rescaled_sbus_data.preflight_debug == 1){

        return StateID::SENSOR_TEST_STATE;
    }

    // サーボ・モーターテスト
    if(context.rescaled_sbus_data.preflight_debug == 2){

        return StateID::SERVO_TEST_STATE;
    }

    // 遷移せず現在の状態を継続
    return StateID::PRE_FLIGHT_STATE;
}


StateID PreFlightState::getStateID() const {

    return StateID::PRE_FLIGHT_STATE;
}
