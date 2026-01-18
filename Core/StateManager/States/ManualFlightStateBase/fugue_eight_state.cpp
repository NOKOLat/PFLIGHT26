#include "States/ManualFlightStateBase/fugue_eight_state.hpp"
#include "StateContext/context.hpp"


void FugueEightState::onEnter(StateContext& context) {

    // フュージュエイト状態固有の初期化処理
}


void FugueEightState::onExit(StateContext& context) {

    // フュージュエイト状態固有のクリーンアップ処理
}


StateResult FugueEightState::onUpdate(StateContext& context) {

    // フュージュエイト用の更新処理

    StateResult result;
    result.success = true;
    result.should_transition = false;
    result.next_state_id = StateID::FUGUE_EIGHT_STATE;

    return result;
}


StateID FugueEightState::getStateID() const {

    return StateID::FUGUE_EIGHT_STATE;
}
