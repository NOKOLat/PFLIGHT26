#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


void LevelTurnState::onEnter(StateContext& context) {

    // レベルターン状態固有の初期化処理
}


void LevelTurnState::onExit(StateContext& context) {

    // レベルターン状態固有のクリーンアップ処理
}


StateResult LevelTurnState::onUpdate(StateContext& context) {

    // レベルターン用の更新処理

    StateResult result;
    result.success = true;
    result.should_transition = false;
    result.next_state_id = StateID::LEVEL_TURN_STATE;

    return result;
}


StateID LevelTurnState::getStateID() const {

    return StateID::LEVEL_TURN_STATE;
}
