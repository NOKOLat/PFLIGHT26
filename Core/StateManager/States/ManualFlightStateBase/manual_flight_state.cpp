#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


ProcessStatus ManualFlightState::onUpdate(StateContext& context) {

    // GPIO設定
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);

    // 手動飛行用の更新処理
    // 制御は不要なので、sbusの値をそのまま出力に反映させる

    // SBUSの値(0~100)をそのままスロットルの値(0~100)に入れる
    context.control_output.motor_pwm[static_cast<int>(PwmConfig::MotorChannel::RIGHT)] = context.rescaled_sbus_data.throttle;
    context.control_output.motor_pwm[static_cast<int>(PwmConfig::MotorChannel::LEFT)]  = context.rescaled_sbus_data.throttle;

    // SBUSの値（-100~100）をサーボの角度（-90~90)に変換
    context.control_output.servo_pwm[static_cast<int>(PwmConfig::ServoChannel::ELEVATOR)]    = context.rescaled_sbus_data.elevator * 0.9;
    context.control_output.servo_pwm[static_cast<int>(PwmConfig::ServoChannel::RUDDER)]      = context.rescaled_sbus_data.rudder * 0.9;
    context.control_output.servo_pwm[static_cast<int>(PwmConfig::ServoChannel::AILERON_L)]   = context.rescaled_sbus_data.aileron * 0.9;

    // 投下装置
    if(context.rescaled_sbus_data.drop == 1) {

        context.control_output.servo_pwm[static_cast<int>(PwmConfig::ServoChannel::DROP)] = 90.0f;
    }
    else {

        context.control_output.servo_pwm[static_cast<int>(PwmConfig::ServoChannel::DROP)] = 0.0f;
    }

    // debug: モーター出力[%], サーボ角度[deg]
    //printf("motor: %f %f | servo: %f %f %f %f %f \n",context.control_output.motor_pwm[static_cast<int>(PwmConfig::MotorChannel::RIGHT)], context.control_output.motor_pwm[static_cast<int>(PwmConfig::MotorChannel::LEFT)], context.control_output.servo_pwm[static_cast<int>(PwmConfig::ServoChannel::AILERON_L)], context.control_output.servo_pwm[static_cast<int>(PwmConfig::ServoChannel::RUDDER)], context.control_output.servo_pwm[static_cast<int>(PwmConfig::ServoChannel::ELEVATOR)], context.control_output.servo_pwm[static_cast<int>(PwmConfig::ServoChannel::DROP)]);

    return ProcessStatus::SUCCESS;
}


StateID ManualFlightState::evaluateNextState(StateContext& context) {

    // ManualFlightState から他の状態への遷移判定をここに実装
    // デフォルトは現在の状態を継続

    // 安全スティックの値を確認（飛行終了判定）
    if(!context.rescaled_sbus_data.safety){

        return StateID::POST_FLIGHT_STATE;
    }

    // 手動飛行の判定
	if(context.rescaled_sbus_data.autofly == 0 && context.rescaled_sbus_data.flight_debug == 0 && context.rescaled_sbus_data.preflight_debug == 0){

	    return StateID::MANUAL_FLIGHT_STATE;
	}

	// デバックモード
	if(context.rescaled_sbus_data.autofly == 0 && context.rescaled_sbus_data.flight_debug >= 1){

		return StateID::LEVEL_FLIGHT_STATE;
	}

	if(context.rescaled_sbus_data.autofly >= 1){

		// 水平旋回
		printf("switch %d\n", context.rescaled_sbus_data.selectmission);
		if(context.rescaled_sbus_data.selectmission == 0){

			return StateID::LEVEL_TURN_STATE;
		}

		if(context.rescaled_sbus_data.selectmission == 1){

			return StateID::CLIMBING_TURN_STATE;
		}

		if(context.rescaled_sbus_data.selectmission == 2){

			return StateID::FUGUE_EIGHT_STATE;
		}

	}

    return StateID::MANUAL_FLIGHT_STATE;
}


StateID ManualFlightState::getStateID() const {

    return StateID::MANUAL_FLIGHT_STATE;
}
