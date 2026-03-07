#include "../state_base.hpp"
#include "StateContext/context.hpp"

StateResult ManualFlightStateBase::update(StateContext& context) {

    // 共通の更新処理

    // すでにsbusとセンサーデータは更新済み（state_manager.cpp)

    // 0. 状態の判定（SBUSのデータを用いて判定）
    StateID next_state = evaluateNextState(context);

    // 状態遷移が必要な場合は、遷移フラグを立てて次の状態IDをセット
    if(next_state != getStateID()) {

        return {ProcessStatus::SUCCESS, TransitionFlag::SHOULD_TRANSITION, next_state};
    }

    // 1. 姿勢推定の更新（ManualFlightStateBase でのみ実行）
    float dt = context.loop_time_us / 1000000.0f;
    if (!context.attitude_estimation.update(dt)) {
        printf("[ManualFlightStateBase::update] Attitude Estimation Update Failed\n");
    }
    context.attitude_state = context.attitude_estimation.getAttitudeState();

    // 2. 派生クラス固有の更新処理を呼び出す（制御出力計算）
    ProcessStatus status = onUpdate(context);

    // 6. PWM出力（PwmManager経由）
    context.instances.pwm_controller->setMotorSpeed(context.control_output.motor_pwm.getptr());
    context.instances.pwm_controller->setServoAngle(context.control_output.servo_pwm.getptr());

	// debug センサーデータの確認
	//printf("Accel: %f, %f, %f\n", context.sensor_data.accel[Axis::X], context.sensor_data.accel[Axis::Y], context.sensor_data.accel[Axis::Z]);

    // debug 角度データの確認
    //printf("Angle: %f, %f %f\n", context.sensor_data.angle[Axis::X], context.sensor_data.angle[Axis::Y], context.sensor_data.angle[Axis::Z]);

    // debug PWMデータの確認
    //printf("PWM: %f, %f, %f, %f\n", context.control_output.servo_pwm.elevator(), context.control_output.servo_pwm.rudder(), context.control_output.servo_pwm.aileron(), context.control_output.servo_pwm.drop());

    return {status, TransitionFlag::NO_TRANSITION, getStateID()};
}


StateBaseID ManualFlightStateBase::getStateBaseID() const {

    return StateBaseID::MANUAL_FLIGHT_STATE_BASE;
}
