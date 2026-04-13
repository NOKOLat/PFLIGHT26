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

    // サーボはこの状態から動くようにする

    // SBUSの値（-100~100）をサーボの角度（-90~90)に変換
    // トリムなし: SBUS_MIN/MID/MAX のデフォルト値から計算する（手動操縦と同じ）
    const auto& raw = context.rescaled_sbus_data.raw_data;
    const float deg_per_pct = context.unit_conversion.SBUS_TO_SERVO_DEG;

    const nokolat::SBUSRescaler::AxisCalib std_calib = {
        nokolat::SBUSRescaler::SBUS_MIN,
        nokolat::SBUSRescaler::SBUS_MID,
        nokolat::SBUSRescaler::SBUS_MAX
    };

    // ここリファクタしたい
    context.control_output.servo_pwm.elevator()      = nokolat::SBUSRescaler::rescaleControl(raw[SbusConfig::CH_ELEVATOR],      std_calib) * deg_per_pct;
    context.control_output.servo_pwm.rudder()        = nokolat::SBUSRescaler::rescaleControl(raw[SbusConfig::CH_RUDDER],        std_calib) * deg_per_pct;
    context.control_output.servo_pwm.left_aileron()  = nokolat::SBUSRescaler::rescaleControl(raw[SbusConfig::CH_AILERON],       std_calib) * deg_per_pct;
    context.control_output.servo_pwm.right_aileron() = nokolat::SBUSRescaler::rescaleControl(raw[SbusConfig::CH_RIGHT_AILERON], std_calib) * deg_per_pct;

    // サーボ出力を定義
    context.instances.pwm_controller->setServoAngle(context.control_output.servo_pwm.getptr());

    // デバック: サーボのデータ
    if(1){

    	printf("Servo: ele: %f, rud: %f, ailL: %f, ailR: %f\n",
    			context.control_output.servo_pwm.elevator(),
    			context.control_output.servo_pwm.rudder(),
    			context.control_output.servo_pwm.left_aileron(),
    			context.control_output.servo_pwm.right_aileron());
    }


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
