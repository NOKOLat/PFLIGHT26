#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"

ProcessStatus PreFlightState::onUpdate(StateContext& context) {

    // デバッグ: SBUS生データを表示
    if(0){

        printf("SBUS Raw: [0]=%d [1]=%d [2]=%d [3]=%d [5]=%d\n",
               context.rescaled_sbus_data.raw_data[0], 
               context.rescaled_sbus_data.raw_data[1], 
               context.rescaled_sbus_data.raw_data[2], 
               context.rescaled_sbus_data.raw_data[3],
			   context.rescaled_sbus_data.raw_data[5]);
    }

    // デバック: サーボのデータ
    if(1){

    	printf("Servo: ele: %f, rud: %f, ailL: %f, ailR: %f\n",
    			context.control_output.servo_pwm.elevator(),
    			context.control_output.servo_pwm.rudder(),
    			context.control_output.servo_pwm.left_aileron(),
    			context.control_output.servo_pwm.right_aileron());
    }

    // サーボはこの状態から動くようにする
    
    // SBUSの値（-100~100）をサーボの角度（-90~90)に変換
    context.control_output.servo_pwm.elevator()      = context.rescaled_sbus_data.elevator * context.unit_conversion.SBUS_TO_SERVO_DEG;
    context.control_output.servo_pwm.rudder()        = context.rescaled_sbus_data.rudder   * context.unit_conversion.SBUS_TO_SERVO_DEG;
    context.control_output.servo_pwm.left_aileron()  = context.rescaled_sbus_data.aileron  * context.unit_conversion.SBUS_TO_SERVO_DEG;
    context.control_output.servo_pwm.right_aileron() = context.rescaled_sbus_data.right_aileron  * context.unit_conversion.SBUS_TO_SERVO_DEG;

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
