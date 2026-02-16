#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


ProcessStatus LevelTurnState::onUpdate(StateContext& context) {

    // レベルターン用の更新処理
    return ProcessStatus::SUCCESS;
}


StateID LevelTurnState::evaluateNextState(StateContext& context) {

    // LevelTurnState から他の状態への遷移判定をここに実装
    // デフォルトは現在の状態を継続
    return StateID::LEVEL_TURN_STATE;
}


StateID LevelTurnState::getStateID() const {

    return StateID::LEVEL_TURN_STATE;
}
