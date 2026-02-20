#include "StateBase/state_base.hpp"
#include "StateContext/context.hpp"


StateResult InitStateBase::update(StateContext& context) {

    // 派生クラス固有の更新処理を呼び出す
    ProcessStatus status = onUpdate(context);

    // 処理が失敗した場合はEMERGENCY_STATEに遷移
    if(status == ProcessStatus::FAILURE) {

        printf("Error: InitStateBase update failed.\n");
        return {ProcessStatus::FAILURE, TransitionFlag::SHOULD_TRANSITION, StateID::EMERGENCY_STATE};
    }

    // 遷移判定
    StateID next_state = evaluateNextState(context);

    return {ProcessStatus::SUCCESS, TransitionFlag::SHOULD_TRANSITION, next_state};
}


StateBaseID InitStateBase::getStateBaseID() const {

    return StateBaseID::INIT_STATE_BASE;
}
