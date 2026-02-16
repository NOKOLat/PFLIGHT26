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
    return {true, false, StateID::LEVEL_TURN_STATE};
}


StateID LevelTurnState::evaluateNextState(StateContext& context) {

    // LevelTurnState から他の状態への遷移判定をここに実装
    // デフォルトは現在の状態を継続
    return StateID::LEVEL_TURN_STATE;
}


StateID LevelTurnState::getStateID() const {

    return StateID::LEVEL_TURN_STATE;
}
