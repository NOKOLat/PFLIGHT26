#include "StateBase/state_base.hpp"
#include "StateContext/context.hpp"


StateResult InitStateBase::update(StateContext& context) {

    // 派生クラス固有の更新処理を呼び出す
    StateResult result = onUpdate(context);

    // 処理が失敗した場合はEMERGENCY_STATEに遷移
    if(!result.success){

        printf("Error: InitStateBase update failed. %s\n", result.error_message);
        return {false, false, StateID::EMERGENCY_STATE};
    }

    // 遷移判定
    StateID next_state = evaluateNextState(context);

    // 現在の状態と次の状態が異なる場合は遷移フラグを立てる
    result.next_state_id = next_state;
    result.should_transition = (next_state != getStateID());

    return result;
}


StateBaseID InitStateBase::getStateBaseID() const {

    return StateBaseID::INIT_STATE_BASE;
}
