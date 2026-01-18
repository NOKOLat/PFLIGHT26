#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


void ClimbingTurnState::onEnter(StateContext& context) {

    // クライミングターン状態固有の初期化処理
}


void ClimbingTurnState::onExit(StateContext& context) {

    // クライミングターン状態固有のクリーンアップ処理
}


StateResult ClimbingTurnState::onUpdate(StateContext& context) {

    // クライミングターン用の更新処理

    StateResult result;
    result.success = true;
    result.should_transition = false;
    result.next_state_id = StateID::CLIMBING_TURN_STATE;

    return result;
}


StateID ClimbingTurnState::getStateID() const {

    return StateID::CLIMBING_TURN_STATE;
}
