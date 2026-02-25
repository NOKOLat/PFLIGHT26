#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"

ProcessStatus PreFlightState::onUpdate(StateContext& context) {

    loop_count++;
	if(loop_count % 10 == 0){

		printf("Channel[3]: %f SBUS[9] = %d\n", context.rescaled_sbus_data.throttle, context.rescaled_sbus_data.safety);
	}

    // サーボはこの状態から動くようにする
    // SBUSの値（-100~100）をサーボの角度（-90~90)に変換
    context.control_output.servo_pwm[static_cast<int>(PwmConfig::ServoChannel::ELEVATOR)]    = context.rescaled_sbus_data.elevator * 0.9;
    context.control_output.servo_pwm[static_cast<int>(PwmConfig::ServoChannel::RUDDER)]      = context.rescaled_sbus_data.rudder * 0.9;
    context.control_output.servo_pwm[static_cast<int>(PwmConfig::ServoChannel::AILERON_L)]   = context.rescaled_sbus_data.aileron * 0.9;

    // PWMを出力（PreFlightStateBaseではpwmを共通処理で出力しないため、ここで記述）
    context.instances.pwm_controller->setServoAngle(context.control_output.servo_pwm.data());

    return ProcessStatus::SUCCESS;
}


StateID PreFlightState::evaluateNextState(StateContext& context) {

    // 手動飛行モード
    if(context.rescaled_sbus_data.safety && context.rescaled_sbus_data.preflight_debug == 0 && context.rescaled_sbus_data.autofly == 0){

        return StateID::MANUAL_FLIGHT_STATE;
    }

    // センサーテスト
    if(context.rescaled_sbus_data.preflight_debug == 1){

        return StateID::SENSOR_TEST_STATE;
    }

    // サーボ・モーターテスト
    if(context.rescaled_sbus_data.preflight_debug == 2){

        return StateID::SERVO_TEST_STATE;
    }

    // 遷移せず現在の状態を継続
    return StateID::PRE_FLIGHT_STATE;
}


StateID PreFlightState::getStateID() const {

    return StateID::PRE_FLIGHT_STATE;
}
