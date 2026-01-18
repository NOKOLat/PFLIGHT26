#include "StateBase/state_base.hpp"
#include "StateContext/context.hpp"


void InitStateBase::enter(StateContext& context) {

    // 共通の初期化処理

    // 派生クラス固有の初期化処理を呼び出す
    onEnter(context);
}


StateResult InitStateBase::update(StateContext& context) {

    // 共通の更新処理

    // 派生クラス固有の更新処理を呼び出す
    StateResult result = onUpdate(context);

    return result;
}


void InitStateBase::exit(StateContext& context) {

    // 派生クラス固有のクリーンアップ処理を呼び出す
    onExit(context);

    // 共通のクリーンアップ処理
}


StateBaseID InitStateBase::getStateBaseID() const {

    return StateBaseID::INIT_STATE_BASE;
}
