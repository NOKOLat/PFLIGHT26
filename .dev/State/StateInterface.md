各状態が持つべき基本機能を定義する

## StateInterface
- enter(): 状態に入る際に実行される初期化処理を定義する純粋仮想関数。
- exit(): 状態から退出する際に実行されるクリーンアップ処理を定義する純粋仮想関数。
- update(): 状態がアクティブな間に定期的に呼び出される処理を定義する純粋仮想関数。
- getStateID(): 現在の状態の名前を取得するための純粋仮想関数。
- getStateBaseID(): 状態が属する基本クラスの情報を取得するための純粋仮想関数。

enter, exit, updateの各関数は、StateContextという構造体を引数として受け取ります。StateContextには、センサーデータや姿勢角、制御用クラスのインスタンスなど、状態遷移や状態内の処理で必要となる情報が含まれています。

enter,exit,updateの関数はstate_result.hppで定義されたStateResult列挙型を返します。StateResultは、状態の処理結果を示すために使用され、以下の値が含まれています。
- 処理の成否
- 状態遷移の有無
- 次の状態のID（状態遷移が発生がない場合は現在の状態IDを返す）


## StateID.hpp
```cpp
#ifndef STATE_ID_HPP
#define STATE_ID_HPP

#include <cstdint>

enum class StateID : uint8_t {

    // InitStateBase
    INIT_STATE = 1,

    // PreFlightStateBase
    CALIBRATION_STATE = 2,
    PRE_FLIGHT_STATE = 3,

    // ManualFlightStateBase
    MANUAL_FLIGHT_STATE = 4,
    LEVEL_TURN_STATE = 5,
    CLIMBING_TURN_STATE = 6,
    FUGUE_EIGHT_STATE = 7,

    // AutoFlightStateBase
    PRE_AUTO_FLIGHT_STATE = 8,
    AUTO_TAKEOFF_STATE = 9,
    AUTO_FLIGHT_STATE = 10,
    AUTO_LANDING_STATE = 11,

    // PostFlightStateBase
    POST_FLIGHT_STATE = 12,
    EMERGENCY_STATE = 13,

    // Invalid
    INVALID_STATE = 255
};

enum class StateBaseID : uint8_t {

    INIT_STATE_BASE = 1,
    PRE_FLIGHT_STATE_BASE = 2,
    MANUAL_FLIGHT_STATE_BASE = 3,
    AUTO_FLIGHT_STATE_BASE = 4,
    POST_FLIGHT_STATE_BASE = 5,
    INVALID_STATE_BASE = 255
};

#endif // STATE_ID_HPP
```


## StateResult.hpp
```cpp
#ifndef STATE_RESULT_HPP
#define STATE_RESULT_HPP

#include "state_id.hpp"


// 状態処理の結果を表す構造体
struct StateResult {

    // 処理の成否
    bool success;

    // 状態遷移の有無
    bool should_transition;

    // 次の状態ID（should_transition == true の時のみ有効）
    StateID next_state_id;

    // エラーメッセージ（デバッグ用）
    const char* error_message = nullptr;
};

#endif // STATE_RESULT_HPP
```

## StateInterface.hpp
```cpp
#ifndef STATE_INTERFACE_HPP
#define STATE_INTERFACE_HPP

#include "state_id.hpp"
#include "state_result.hpp"


// 前方宣言
struct StateContext;


// 各状態が実装すべき基本インターフェース
class StateInterface {

    public:

        virtual ~StateInterface() = default;

        // 状態に入る際に呼ばれる初期化処理
        virtual void enter(StateContext& context) = 0;

        // 状態がアクティブな間に定期的に呼ばれる処理
        virtual StateResult update(StateContext& context) = 0;

        // 状態から退出する際に呼ばれるクリーンアップ処理
        virtual void exit(StateContext& context) = 0;

        // 現在の状態IDを取得する
        virtual StateID getStateID() const = 0;

        // 状態が属する基底クラスのIDを取得する
        virtual StateBaseID getStateBaseID() const = 0;
};

#endif // STATE_INTERFACE_HPP
```