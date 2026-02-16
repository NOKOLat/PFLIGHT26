#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


ProcessStatus ClimbingTurnState::onUpdate(StateContext& context) {

    // クライミングターン用の更新処理
    return ProcessStatus::SUCCESS;
}


StateID ClimbingTurnState::evaluateNextState(StateContext& context) {

    // ClimbingTurnState から他の状態への遷移判定をここに実装
    // デフォルトは現在の状態を継続
    return StateID::CLIMBING_TURN_STATE;
}


StateID ClimbingTurnState::getStateID() const {

    return StateID::CLIMBING_TURN_STATE;
}
