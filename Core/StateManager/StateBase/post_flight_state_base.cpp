#include "StateBase/state_base.hpp"
#include "StateContext/context.hpp"


void PostFlightStateBase::enter(StateContext& context) {

    // 共通の初期化処理

    // モーターの停止処理
    // context.motor_driver->stop();

    // 派生クラス固有の初期化処理を呼び出す
    onEnter(context);
}


StateResult PostFlightStateBase::update(StateContext& context) {

    // 共通の更新処理

    // 派生クラス固有の更新処理を呼び出す
    StateResult result = onUpdate(context);

    // 遷移判定
    StateID next_state = evaluateNextState(context);
    result.next_state_id = next_state;
    result.should_transition = (next_state != getStateID());

    return result;
}


void PostFlightStateBase::exit(StateContext& context) {

    // 派生クラス固有のクリーンアップ処理を呼び出す
    onExit(context);

    // 共通のクリーンアップ処理
}


StateBaseID PostFlightStateBase::getStateBaseID() const {

    return StateBaseID::POST_FLIGHT_STATE_BASE;
}
