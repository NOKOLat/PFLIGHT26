#include "StateBase/state_base.hpp"
#include "StateContext/context.hpp"


StateResult PreFlightStateBase::update(StateContext& context) {

    // 派生クラス固有の更新処理を呼び出す
    StateResult result = onUpdate(context);

    // 遷移判定
    StateID next_state = evaluateNextState(context);
    result.next_state_id = next_state;
    result.should_transition = (next_state != getStateID());

    return result;
}


StateBaseID PreFlightStateBase::getStateBaseID() const {

    return StateBaseID::PRE_FLIGHT_STATE_BASE;
}
