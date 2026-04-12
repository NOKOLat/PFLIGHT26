#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


ProcessStatus ManualFlightState::onUpdate(StateContext& context) {

    // GPIO設定
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);

    // 手動飛行用の更新処理
    // 制御は不要なので、sbusの値をそのまま出力に反映させる

    // SBUSの値(0~100)をそのままスロットルの値(0~100)に入れる
    context.control_output.motor_pwm.right() = context.rescaled_sbus_data.throttle;
    context.control_output.motor_pwm.left()  = context.rescaled_sbus_data.throttle;

    // SBUSの値（-100~100）をサーボの角度（-90~90)に変換
    context.control_output.servo_pwm.elevator()      = context.rescaled_sbus_data.elevator * context.unit_conversion.SBUS_TO_SERVO_DEG;
    context.control_output.servo_pwm.rudder()        = context.rescaled_sbus_data.rudder   * context.unit_conversion.SBUS_TO_SERVO_DEG;
    context.control_output.servo_pwm.left_aileron()  = context.rescaled_sbus_data.aileron  * context.unit_conversion.SBUS_TO_SERVO_DEG;
    context.control_output.servo_pwm.right_aileron() = context.rescaled_sbus_data.right_aileron * context.unit_conversion.SBUS_TO_SERVO_DEG;


    // 投下装置
    if(context.rescaled_sbus_data.drop == SwitchPosition::MID) {

        context.control_output.servo_pwm.drop() = 90.0f;
    }
    else {

        context.control_output.servo_pwm.drop() = 0.0f;
    }

    // debug: モーター出力[%], サーボ角度[deg]
    // printf("motor: %f %f | servo: %f %f %f %f\n",
    //         context.control_output.motor_pwm.right(),
    //         context.control_output.motor_pwm.left(),
    //         context.control_output.servo_pwm.elevator(),
    //         context.control_output.servo_pwm.rudder(),
    //         context.control_output.servo_pwm.aileron(),
    //         context.control_output.servo_pwm.drop()
    // );

    return ProcessStatus::SUCCESS;
}


StateID ManualFlightState::evaluateNextState(StateContext& context) {

    // ManualFlightState から他の状態への遷移判定をここに実装
    // デフォルトは現在の状態を継続

    // 安全スティックの値を確認（飛行終了判定）
    if(context.rescaled_sbus_data.safety == SwitchPosition::LOW){

        return StateID::POST_FLIGHT_STATE;
    }

    // 手動飛行の判定
	if(context.rescaled_sbus_data.autofly == SwitchPosition::LOW && context.rescaled_sbus_data.flight_debug == SwitchPosition::LOW && context.rescaled_sbus_data.preflight_debug == SwitchPosition::LOW){

        return StateID::MANUAL_FLIGHT_STATE;
	}

	// デバックモード
	if(context.rescaled_sbus_data.autofly == SwitchPosition::LOW && context.rescaled_sbus_data.flight_debug != SwitchPosition::LOW){

		return StateID::LEVEL_FLIGHT_STATE;
	}

	if(context.rescaled_sbus_data.autofly != SwitchPosition::LOW){

		if(context.rescaled_sbus_data.selectmission == SwitchPosition::LOW){

			return StateID::LEVEL_TURN_STATE;
		}

		if(context.rescaled_sbus_data.selectmission == SwitchPosition::MID){

			return StateID::CLIMBING_TURN_STATE;
		}

		if(context.rescaled_sbus_data.selectmission == SwitchPosition::HIGH){

			return StateID::FUGUE_EIGHT_STATE;
		}

	}

    return StateID::MANUAL_FLIGHT_STATE;
}


StateID ManualFlightState::getStateID() const {

    return StateID::MANUAL_FLIGHT_STATE;
}
