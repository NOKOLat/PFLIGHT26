#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


void PreFlightState::onEnter(StateContext& context) {

    // 飛行前の準備処理の初期化
}


StateResult PreFlightState::onUpdate(StateContext& context) {

    // 飛行前の準備処理
    printf("Wait Start SBUS[9] = %d, %f\n", context.rescaled_sbus_data.aux5, context.rescaled_sbus_data.throttle);

    return {true, false, StateID::PRE_FLIGHT_STATE};
}


void PreFlightState::onExit(StateContext& context) {

    // クリーンアップ処理
}


StateID PreFlightState::evaluateNextState(StateContext& context) {

    // 安全スティックの値を確認
    if(context.rescaled_sbus_data.aux5){

        printf("Start SBUS[9] = %d\n", context.rescaled_sbus_data.aux5);
        return StateID::MANUAL_FLIGHT_STATE;
    }

    // 遷移せず現在の状態を継続
    return StateID::PRE_FLIGHT_STATE;
}


StateID PreFlightState::getStateID() const {

    return StateID::PRE_FLIGHT_STATE;
}
