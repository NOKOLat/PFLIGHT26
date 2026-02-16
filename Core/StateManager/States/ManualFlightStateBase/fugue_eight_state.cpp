#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


ProcessStatus FugueEightState::onUpdate(StateContext& context) {

    // フュージュエイト用の更新処理
    return ProcessStatus::SUCCESS;
}


StateID FugueEightState::evaluateNextState(StateContext& context) {

    // FugueEightState から他の状態への遷移判定をここに実装
    // デフォルトは現在の状態を継続
    return StateID::FUGUE_EIGHT_STATE;
}


StateID FugueEightState::getStateID() const {

    return StateID::FUGUE_EIGHT_STATE;
}
