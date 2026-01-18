#include "States/PostFlightStateBase/emergency_state.hpp"
#include "StateContext/context.hpp"


void EmergencyState::onEnter(StateContext& context) {

    // 緊急状態固有の初期化処理
    // モーターの即座停止
    // context.motor_driver->emergencyStop();
}


void EmergencyState::onExit(StateContext& context) {

    // 緊急状態固有のクリーンアップ処理
}


StateResult EmergencyState::onUpdate(StateContext& context) {

    // 緊急対応の処理

    StateResult result;
    result.success = true;
    result.should_transition = false;
    result.next_state_id = StateID::EMERGENCY_STATE;

    return result;
}


StateID EmergencyState::getStateID() const {

    return StateID::EMERGENCY_STATE;
}
