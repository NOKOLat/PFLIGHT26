#include "StateBase/state_base.hpp"
#include "StateContext/context.hpp"


StateResult AutoFlightStateBase::update(StateContext& context) {

    // 共通の更新処理

    // センサーデータの取得
    // context.imu_sensor->readData(context.sensor_data);
    // context.mag_sensor->readData(context.sensor_data);

    // LiDARデータの取得
    // context.lidar_sensor->readData(context.sensor_data);

    // EKFの更新
    // context.ekf->update(context.sensor_data, context.attitude);

    // 派生クラス固有の更新処理を呼び出す
    ProcessStatus status = onUpdate(context);

    if (status == ProcessStatus::FAILURE) {
        
        return {ProcessStatus::FAILURE, TransitionFlag::NO_TRANSITION, getStateID()};
    }

    // PWM値の出力
    // context.motor_driver->setPWM(context.control_output.motor_pwm);

    // 遷移判定
    StateID next_state = evaluateNextState(context);
    TransitionFlag should_transition = (next_state != getStateID()) ? TransitionFlag::SHOULD_TRANSITION : TransitionFlag::NO_TRANSITION;

    return {status, should_transition, next_state};
}


StateBaseID AutoFlightStateBase::getStateBaseID() const {

    return StateBaseID::AUTO_FLIGHT_STATE_BASE;
}
