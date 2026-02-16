#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


void PostFlightState::onEnter(StateContext& context) {

    // 飛行後状態固有の初期化処理
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
}


void PostFlightState::onExit(StateContext& context) {

    // 飛行後状態固有のクリーンアップ処理
}


StateResult PostFlightState::onUpdate(StateContext& context) {

    // 飛行後の処理
    HAL_Delay(1000);

    return {true, false, StateID::POST_FLIGHT_STATE};
}


StateID PostFlightState::evaluateNextState(StateContext& context) {

    // 飛行後の処理完了後はPRE_FLIGHT_STATEに遷移
    return StateID::PRE_FLIGHT_STATE;
}


StateID PostFlightState::getStateID() const {

    return StateID::POST_FLIGHT_STATE;
}
