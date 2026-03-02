#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"

ProcessStatus PreFlightState::onUpdate(StateContext& context) {

    loop_count++;
	if(loop_count % 10 == 0){

		printf("Channel[3]: %f SBUS[9] = %d\n", context.rescaled_sbus_data.throttle, static_cast<int>(context.rescaled_sbus_data.safety));
	}

    // サーボはこの状態から動くようにする
    
    // SBUSの値（-100~100）をサーボの角度（-90~90)に変換
    context.control_output.servo_pwm.elevator() = context.rescaled_sbus_data.elevator * 0.9f;
    context.control_output.servo_pwm.rudder()   = context.rescaled_sbus_data.rudder   * 0.9f;
    context.control_output.servo_pwm.aileron()  = context.rescaled_sbus_data.aileron  * 0.9f;

    // サーボ出力を定義
    context.instances.pwm_controller->setServoAngle(context.control_output.servo_pwm.getptr());


    return ProcessStatus::SUCCESS;
}


StateID PreFlightState::evaluateNextState(StateContext& context) {

    // 手動飛行モード（スイッチ判定）
    if(context.rescaled_sbus_data.safety != SwitchPosition::LOW && context.rescaled_sbus_data.preflight_debug == SwitchPosition::LOW && context.rescaled_sbus_data.autofly == SwitchPosition::LOW){

        // スロットが一定以上の場合は遷移しない
        if(context.rescaled_sbus_data.throttle < 400){

            return StateID::MANUAL_FLIGHT_STATE;
        }
        
        return StateID::PRE_FLIGHT_STATE;
    }

    // センサーテスト
    if(context.rescaled_sbus_data.preflight_debug == SwitchPosition::MID){

        return StateID::SENSOR_TEST_STATE;
    }

    // サーボ・モーターテスト
    if(context.rescaled_sbus_data.preflight_debug == SwitchPosition::HIGH){

        return StateID::SERVO_TEST_STATE;
    }

    // 遷移せず現在の状態を継続
    return StateID::PRE_FLIGHT_STATE;
}


StateID PreFlightState::getStateID() const {

    return StateID::PRE_FLIGHT_STATE;
}
