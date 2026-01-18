複数の状態で共通する処理をまとめて実装する基底クラスの実装

- StateInterfaceを継承し、各状態が持つべき基本機能を実装する。
- 各状態クラスはこの基底クラスを継承し、共通の動作を利用できる。

## 基本設計

StateInterfaceで用意された純粋仮想関数をオーバーライドし、共通の動作を実装する。
派生クラス用の実装は、ここで作成した純粋仮想関数onEnter、onExit、onUpdateを呼び出す形で行う。

- Enter(): 共通実装 + OnEnter()（派生クラスでの追加処理）
- Exit(): 共通実装 + OnExit()（派生クラスでの追加処理）
- Update(): 共通実装 + OnUpdate()（派生クラスでの追加処理）
- GetStateID(): 派生クラスで状態名を返す実装を行う（派生クラスが直接オーバーライド）
- GetStateBaseID(): 派生クラスで基底クラス名を返す実装を行う（このクラスで実装）

## InitStateBase

特にやることがない

## PreFlightStateBase

特にやることがない

## ManualFlightStateBase

1. センサーデータの取得
2. EKFの更新
（派生クラス: 目標値と現在値を使ったPID制御計算）
3. 派生クラスが計算したPWM値の出力

## AutoFlightStateBase

1. センサーデータの取得
2. LiDARデータの取得
3. EKFの更新
(派生クラス: 目標値と現在値を使ったPID制御計算）
4. 派生クラスが計算したPWM値の出力)

## PostFlightStateBase

1. モーターの停止処理

---

## ManualFlightStateBase の実装

### ManualFlightStateBase.hpp

```cpp
#ifndef MANUAL_FLIGHT_STATE_BASE_HPP
#define MANUAL_FLIGHT_STATE_BASE_HPP

#include "StateInterface/state_interface.hpp"
#include "StateInterface/state_id.hpp"
#include "StateInterface/state_result.hpp"


// 前方宣言
struct StateContext;


// 手動飛行の共通処理を実装する基底クラス
class ManualFlightStateBase : public StateInterface {

    protected:

        // 派生クラスで実装すべき処理
        // 派生クラス固有の初期化処理
        virtual void onEnter(StateContext& context) = 0;

        // 派生クラス固有のクリーンアップ処理
        virtual void onExit(StateContext& context) = 0;

        // 派生クラス固有の更新処理（PID計算など）
        virtual StateResult onUpdate(StateContext& context) = 0;


    public:

        virtual ~ManualFlightStateBase() = default;


        // 状態に入る際に呼ばれる共通初期化処理
        virtual void enter(StateContext& context) override;


        // 状態がアクティブな間に定期的に呼ばれる共通処理
        virtual StateResult update(StateContext& context) override;


        // 状態から退出する際に呼ばれる共通クリーンアップ処理
        virtual void exit(StateContext& context) override;


        // 状態が属する基底クラスのIDを取得する
        virtual StateBaseID getStateBaseID() const override;
};

#endif // MANUAL_FLIGHT_STATE_BASE_HPP
```

### ManualFlightStateBase.cpp

```cpp
#include "manual_flight_state_base.hpp"
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
```
