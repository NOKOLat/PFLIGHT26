#include "StateBase/state_base.hpp"
#include "StateContext/context.hpp"


StateResult PostFlightStateBase::update(StateContext& context) {

    // 派生クラス固有の更新処理を呼び出す
    ProcessStatus status = onUpdate(context);

    // 遷移判定
    StateID next_state = evaluateNextState(context);
    TransitionFlag should_transition = (next_state != getStateID()) ? TransitionFlag::SHOULD_TRANSITION : TransitionFlag::NO_TRANSITION;

    return {status, should_transition, next_state};
}


StateBaseID PostFlightStateBase::getStateBaseID() const {

    return StateBaseID::POST_FLIGHT_STATE_BASE;
}
