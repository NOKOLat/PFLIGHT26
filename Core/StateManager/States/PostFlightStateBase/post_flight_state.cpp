#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


ProcessStatus PostFlightState::onUpdate(StateContext& context) {

    // GPIO設定
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);

    // 飛行後の処理
    HAL_Delay(1000);

    return ProcessStatus::SUCCESS;
}


StateID PostFlightState::evaluateNextState(StateContext& context) {

    // 飛行後の処理完了後はPRE_FLIGHT_STATEに遷移
    return StateID::PRE_FLIGHT_STATE;
}


StateID PostFlightState::getStateID() const {

    return StateID::POST_FLIGHT_STATE;
}
