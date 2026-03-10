#include "StateBase/state_base.hpp"
#include "StateContext/context.hpp"

StateResult ManualFlightStateBase::update(StateContext& context) {

    // 共通の更新処理

    // 0. 状態の判定（SBUSのデータを用いて判定）
    StateID next_state = evaluateNextState(context);

    // 状態遷移が必要な場合は、遷移フラグを立てて次の状態IDをセット
    if(next_state != getStateID()) {

        return {ProcessStatus::SUCCESS, TransitionFlag::SHOULD_TRANSITION, next_state};
    }

    // 1. センサー融合を実行
    if (!context.instances.sensor_fusion_manager.has_value()) {

        printf("[ManualFlightStateBase] Error: SensorFusionManager is not initialized\n");
        return {ProcessStatus::FAILURE, TransitionFlag::NO_TRANSITION, getStateID()};
    }

    context.instances.sensor_fusion_manager->update();

    // 2. 推定結果を StateContext に保存
    context.attitude = context.instances.sensor_fusion_manager->getAttitude();
    context.altitude = context.instances.sensor_fusion_manager->getAltitude();

    // 3. 移動平均の計算（高度、ヨー角）をセンサーマネージャーで実行
    SensorManager* sensor_mgr = context.instances.sensor_fusion_manager->getSensorManager();
    if (sensor_mgr != nullptr) {
        sensor_mgr->updateMovingAverages(context.altitude, context.attitude.yaw());
    }

    // 5. 派生クラス固有の更新処理を呼び出す（制御出力）
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
