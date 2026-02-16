#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


ProcessStatus ManualFlightState::onUpdate(StateContext& context) {

    // GPIO設定
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);

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

    return ProcessStatus::SUCCESS;
}


StateID ManualFlightState::evaluateNextState(StateContext& context) {

    // ManualFlightState から他の状態への遷移判定をここに実装
    // デフォルトは現在の状態を継続
    return StateID::MANUAL_FLIGHT_STATE;
}


StateID ManualFlightState::getStateID() const {

    return StateID::MANUAL_FLIGHT_STATE;
}
