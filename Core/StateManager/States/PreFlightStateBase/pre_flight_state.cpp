#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


void PreFlightState::enter(StateContext& context) {

    // 飛行前の準備処理の初期化
}


StateResult PreFlightState::update(StateContext& context) {

    // 飛行前の準備処理

	// 安全スティックの値を確認
	if(context.rescaled_sbus_data.aux5){

		printf("Start SBUS[9] = %d\n", context.rescaled_sbus_data.aux5);
		return {true, true, StateID::MANUAL_FLIGHT_STATE};
	}

	printf("Wait Start SBUS[9] = %d, %f\n", context.rescaled_sbus_data.aux5, context.rescaled_sbus_data.throttle);

	return {true, false, StateID::PRE_FLIGHT_STATE};
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
