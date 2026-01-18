#include "StateBase/state_base.hpp"
#include "StateContext/context.hpp"


void ManualFlightStateBase::enter(StateContext& context) {

    // 共通の初期化処理

    // PIDコントローラーの初期化
    // context.pid_controller->initialize();

    // センサーキャリブレーション
    // context.imu_sensor->calibrate();

    // 派生クラス固有の初期化処理を呼び出す
    onEnter(context);
}


StateResult ManualFlightStateBase::update(StateContext& context) {

    // 共通の更新処理

    // センサーデータの取得
    // context.imu_sensor->readData(context.sensor_data);
    // context.mag_sensor->readData(context.sensor_data);

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


void ManualFlightStateBase::exit(StateContext& context) {

    // 派生クラス固有のクリーンアップ処理を呼び出す
    onExit(context);

    // 共通のクリーンアップ処理
}


StateBaseID ManualFlightStateBase::getStateBaseID() const {

    return StateBaseID::MANUAL_FLIGHT_STATE_BASE;
}
