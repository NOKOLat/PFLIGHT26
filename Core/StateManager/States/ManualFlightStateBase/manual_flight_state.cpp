#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


void ManualFlightState::onEnter(StateContext& context) {

    // 手動飛行状態固有の初期化処理
    // PID制御の目標値を初期化
    // context.pid_controller->setTarget(0.0f);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);

}


void ManualFlightState::onExit(StateContext& context) {

    // 手動飛行状態固有のクリーンアップ処理
    // 必要に応じてここで状態をリセット
}


StateResult ManualFlightState::onUpdate(StateContext& context) {

    // 手動飛行用の更新処理
    // 制御は不要なので、sbusの値をそのまま出力に反映させる

    context.control_output.motor_pwm[0] = context.rescaled_sbus_data.throttle; // 右モーター
    context.control_output.motor_pwm[1] = context.rescaled_sbus_data.throttle; // 左モーター

    context.control_output.servo_pwm[0] = context.rescaled_sbus_data.elevator; // エレベーター
    context.control_output.servo_pwm[1] = context.rescaled_sbus_data.rudder;   // ラダー
    context.control_output.servo_pwm[2] = context.rescaled_sbus_data.aileron;  // エルロン 
    
    // 投下装置
    if(context.rescaled_sbus_data.aux6 == 1) {

        context.control_output.servo_pwm[3] = 90.0f; 
    } 
    else {

        context.control_output.servo_pwm[3] = 0.0f;
    }

    StateResult result;
    result.success = true;
    result.should_transition = false;
    result.next_state_id = StateID::MANUAL_FLIGHT_STATE;

    return result;
}


StateID ManualFlightState::getStateID() const {

    return StateID::MANUAL_FLIGHT_STATE;
}
