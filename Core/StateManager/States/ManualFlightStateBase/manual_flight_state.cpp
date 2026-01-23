#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


void ManualFlightState::onEnter(StateContext& context) {

    // 手動飛行状態固有の初期化処理
    // PID制御の目標値を初期化
    // context.pid_controller->setTarget(0.0f);
}


void ManualFlightState::onExit(StateContext& context) {

    // 手動飛行状態固有のクリーンアップ処理
    // 必要に応じてここで状態をリセット
}


StateResult ManualFlightState::onUpdate(StateContext& context) {

    // 手動飛行用の更新処理

    // 現在の姿勢情報から目標値を計算
    // （パイロット入力に基づいて）

    // PID制御の計算（ロール角制御）
    // float roll_pwm = context.pid_controller->calculateRoll(target_roll, current_roll);

    // PID制御の計算（ピッチ角制御）
    // float pitch_pwm = context.pid_controller->calculatePitch(target_pitch, current_pitch);

    // PID制御の計算（ヨー角速度制御）
    // float yaw_pwm = context.pid_controller->calculateYaw(target_yaw_rate, current_yaw_rate);

    // モーター出力
    // context.control_output.motor_pwm[0] = static_cast<uint16_t>(roll_pwm);
    // context.control_output.motor_pwm[1] = static_cast<uint16_t>(pitch_pwm);
    // context.control_output.motor_pwm[2] = static_cast<uint16_t>(yaw_pwm);
    // context.control_output.motor_pwm[3] = static_cast<uint16_t>(0);

    StateResult result;
    result.success = true;
    result.should_transition = false;
    result.next_state_id = StateID::MANUAL_FLIGHT_STATE;

    return result;
}


StateID ManualFlightState::getStateID() const {

    return StateID::MANUAL_FLIGHT_STATE;
}
