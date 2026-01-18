# 状態実装例

## InitState

### init_states.hpp

```cpp
#ifndef INIT_STATES_HPP
#define INIT_STATES_HPP

#include "StateInterface/state_interface.hpp"
#include "StateInterface/state_id.hpp"
#include "StateInterface/state_result.hpp"


// 前方宣言
struct StateContext;


// 初期化状態を実装するクラス
class InitState : public StateInterface {

    public:

        virtual ~InitState() = default;


        // 状態に入る際に呼ばれる初期化処理
        virtual void enter(StateContext& context) override;


        // 状態がアクティブな間に定期的に呼ばれる処理
        virtual StateResult update(StateContext& context) override;


        // 状態から退出する際に呼ばれるクリーンアップ処理
        virtual void exit(StateContext& context) override;


        // 現在の状態IDを取得する
        virtual StateID getStateID() const override;


        // 状態が属する基底クラスのIDを取得する
        virtual StateBaseID getStateBaseID() const override;
};

#endif // INIT_STATES_HPP
```

### init_state.cpp

```cpp
#include "States/InitStateBase/init_states.hpp"
#include "StateContext/context.hpp"


void InitState::enter(StateContext& context) {

    // 各インスタンスを emplace で構築（静的メモリ確保）
    context.imu_sensor.emplace();
    context.mag_sensor.emplace();
    context.lidar_sensor.emplace();
    context.barometric_sensor.emplace();
    context.motor_driver.emplace();
    context.pid_controller.emplace();

    // ピン設定の初期化
    context.pin_config.motor_pins[0] = 0;  // PIN_MOTOR_1
    context.pin_config.motor_pins[1] = 1;  // PIN_MOTOR_2
    context.pin_config.motor_pins[2] = 2;  // PIN_MOTOR_3
    context.pin_config.motor_pins[3] = 3;  // PIN_MOTOR_4

    // センサーの初期化
    context.imu_sensor->initialize();
    context.mag_sensor->initialize();
    context.motor_driver->initializePins(context.pin_config);

    // キャリブレーション
    context.imu_sensor->calibrate();
    context.mag_sensor->calibrate();
}


StateResult InitState::update(StateContext& context) {

    // 通信チェック等の処理
    // bool is_connected = context.wireless->isConnected();

    StateResult result;
    result.success = true;
    result.should_transition = true;
    result.next_state_id = StateID::CALIBRATION_STATE;

    return result;
}


void InitState::exit(StateContext& context) {

    // クリーンアップ処理
}


StateID InitState::getStateID() const {

    return StateID::INIT_STATE;
}


StateBaseID InitState::getStateBaseID() const {

    return StateBaseID::INIT_STATE_BASE;
}
```

---

## 状態実装例：ManualFlightState

### manual_flight_states.hpp

```cpp
#ifndef MANUAL_FLIGHT_STATES_HPP
#define MANUAL_FLIGHT_STATES_HPP

#include "StateBase/manual_flight_state_base.hpp"
#include "StateInterface/state_id.hpp"
#include "StateInterface/state_result.hpp"


// 前方宣言
struct StateContext;

// 手動飛行状態を実装するクラス
class ManualFlightState : public ManualFlightStateBase {

    public:

        virtual ~ManualFlightState() = default;


        // 現在の状態IDを取得する
        virtual StateID getStateID() const override;

    private:

        // 派生クラス固有の初期化処理
        virtual void onEnter(StateContext& context) override;

        // 派生クラス固有のクリーンアップ処理
        virtual void onExit(StateContext& context) override;

        // 派生クラス固有の更新処理（手動飛行用のPID計算）
        virtual StateResult onUpdate(StateContext& context) override;
};

#endif // MANUAL_FLIGHT_STATES_HPP
```

### manual_flight_state.cpp

```cpp
#include "States/ManualFlightStateBase/manual_flight_states.hpp"
#include "StateContext/context.hpp"


void ManualFlightState::onEnter(StateContext& context) {

    // 手動飛行状態固有の初期化処理
    // PID制御の目標値を初期化
    // context.pid_controller->setTarget(0.0f);
}


void ManualFlightState::onExit(StateContext& context) {

    // 手動飛行状態固有のクリーンアップ処理
    // 必要に応じてここで状態をリセット
}


StateResult ManualFlightState::onUpdate(StateContext& context) {

    // 手動飛行用の更新処理

    // 現在の姿勢情報から目標値を計算
    // （パイロット入力に基づいて）
    float target_roll = context.attitude.roll;      // 目標ロール角
    float target_pitch = context.attitude.pitch;    // 目標ピッチ角
    float target_yaw_rate = context.attitude.yaw_rate; // 目標ヨー角速度

    // 現在値
    float current_roll = context.attitude.roll;
    float current_pitch = context.attitude.pitch;
    float current_yaw_rate = context.attitude.yaw_rate;

    // PID制御の計算（ロール角制御）
    // float roll_pwm = context.pid_controller->calculateRoll(target_roll, current_roll);

    // PID制御の計算（ピッチ角制御）
    // float pitch_pwm = context.pid_controller->calculatePitch(target_pitch, current_pitch);

    // PID制御の計算（ヨー角速度制御）
    // float yaw_pwm = context.pid_controller->calculateYaw(target_yaw_rate, current_yaw_rate);

    // 制御量を PWM 値に変換してモーターに出力
    // context.control_output.motor_pwm[0] = (uint16_t)roll_pwm;
    // context.control_output.motor_pwm[1] = (uint16_t)pitch_pwm;
    // context.control_output.motor_pwm[2] = (uint16_t)yaw_pwm;
    // context.control_output.motor_pwm[3] = (uint16_t)thrust_pwm;

    StateResult result;
    result.success = true;
    result.should_transition = false;
    result.next_state_id = getStateID();

    return result;
}


StateID ManualFlightState::getStateID() const {

    return StateID::MANUAL_FLIGHT_STATE;
}
```