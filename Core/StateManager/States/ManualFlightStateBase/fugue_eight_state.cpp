#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


void FugueEightState::onEnter(StateContext& context) {

    // フュージュエイト状態固有の初期化処理
}


void FugueEightState::onExit(StateContext& context) {

    // フュージュエイト状態固有のクリーンアップ処理
}


StateResult FugueEightState::onUpdate(StateContext& context) {

    // フュージュエイト用の更新処理
    return {true, false, StateID::FUGUE_EIGHT_STATE};
}


StateID FugueEightState::evaluateNextState(StateContext& context) {

    // FugueEightState から他の状態への遷移判定をここに実装
    // デフォルトは現在の状態を継続
    return StateID::FUGUE_EIGHT_STATE;
}


StateID FugueEightState::getStateID() const {

    return StateID::FUGUE_EIGHT_STATE;
}
