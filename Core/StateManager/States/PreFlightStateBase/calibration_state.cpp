#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


void CalibrationState::enter(StateContext& context) {

    // キャリブレーション処理の初期化
}


StateResult CalibrationState::update(StateContext& context) {

    // キャリブレーション処理

    StateResult result;
    result.success = true;
    result.should_transition = true;
    result.next_state_id = StateID::PRE_FLIGHT_STATE;

    return result;
}


void CalibrationState::exit(StateContext& context) {

    // クリーンアップ処理
}


StateID CalibrationState::getStateID() const {

    return StateID::CALIBRATION_STATE;
}


StateBaseID CalibrationState::getStateBaseID() const {

    return StateBaseID::PRE_FLIGHT_STATE_BASE;
}
