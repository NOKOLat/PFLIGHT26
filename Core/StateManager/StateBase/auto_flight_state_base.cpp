#include "StateBase/state_base.hpp"
#include "StateContext/context.hpp"


void AutoFlightStateBase::enter(StateContext& context) {

    // 共通の初期化処理

    // 派生クラス固有の初期化処理を呼び出す
    onEnter(context);
}


StateResult AutoFlightStateBase::update(StateContext& context) {

    // 共通の更新処理

    // センサーデータの取得
    // context.imu_sensor->readData(context.sensor_data);
    // context.mag_sensor->readData(context.sensor_data);

    // LiDARデータの取得
    // context.lidar_sensor->readData(context.sensor_data);

    // EKFの更新
    // context.ekf->update(context.sensor_data, context.attitude);

    // 派生クラス固有の更新処理を呼び出す（PID計算）
    StateResult result = onUpdate(context);

    if (!result.success) {
        return result;
    }

    // PWM値の出力
    // context.motor_driver->setPWM(context.control_output.motor_pwm);

    return result;
}


void AutoFlightStateBase::exit(StateContext& context) {

    // 派生クラス固有のクリーンアップ処理を呼び出す
    onExit(context);

    // 共通のクリーンアップ処理
}


StateBaseID AutoFlightStateBase::getStateBaseID() const {

    return StateBaseID::AUTO_FLIGHT_STATE_BASE;
}
