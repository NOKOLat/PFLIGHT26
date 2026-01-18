
# StateManager

状態管理を司るクラス。状態遷移、状態の更新、ループ間隔の管理を行う。

---

## state_manager.hpp

```cpp
#ifndef STATE_MANAGER_HPP
#define STATE_MANAGER_HPP

#include <memory>
#include <optional>
#include "StateInterface/state_interface.hpp"
#include "StateInterface/state_id.hpp"
#include "StateContext/context.hpp"
#include "LoopManager/loop_manager.hpp"


// 前方宣言
class IMUSensor;
class MagnetometerSensor;
class LiDARSensor;
class BarometricSensor;
class MotorDriver;
class PIDController;


// 状態管理クラス
class StateManager {

    public:

        // コンストラクタ（初期状態を設定）
        StateManager(StateID init_state_id);

        // デストラクタ
        virtual ~StateManager() = default;

        // メインループの更新処理
        void update();


    private:

        // StateContext をメンバーとして保持
        StateContext state_context_;

        // 現在の状態
        std::unique_ptr<StateInterface> current_state_;

        // ループ管理（10Hz = 100000us）
        LoopManager loop_manager_;

        // 状態遷移
        void changeState(std::unique_ptr<StateInterface> new_state);
};

#endif // STATE_MANAGER_HPP
```

---

## state_manager.cpp

```cpp
#include "StateManager/state_manager.hpp"
#include "StateFactory/state_factory.hpp"


// コンストラクタ
StateManager::StateManager(StateID init_state_id)
    : loop_manager_(100000) {

    // 初期状態を生成
    current_state_ = StateFactory::createState(init_state_id);

    // 初期状態のenter関数を呼ぶ
    if (current_state_) {
        current_state_->enter(state_context_);
    }
}


void StateManager::changeState(std::unique_ptr<StateInterface> new_state) {

    // 現在の状態の終了処理
    if (current_state_) {

        current_state_->exit(state_context_);

        // デバッグ出力
        // printf("[StateManager] StateExit: %d\n", static_cast<int>(current_state_->getStateID()));
    }

    // 新しい状態に遷移
    current_state_ = std::move(new_state);

    // 新しい状態の開始処理
    if (current_state_) {

        current_state_->enter(state_context_);

        // デバッグ出力
        // printf("[StateManager] StateEnter: %d\n", static_cast<int>(current_state_->getStateID()));
    }
}


void StateManager::update() {

    // ループ間隔まで待機が必要な場合は処理を進めない
    if (loop_manager_.isWaitNextLoop()) {
        return;
    }

    // 現在の状態が有効な場合
    if (current_state_) {

        // 現在の状態の更新処理
        StateResult result = current_state_->update(state_context_);

        // 状態遷移が必要かつ処理が成功した場合
        if (result.should_transition && result.success) {

            // 新しい状態を生成
            std::unique_ptr<StateInterface> next_state = StateFactory::createState(result.next_state_id);

            // 状態遷移を実行
            if (next_state) {
                changeState(std::move(next_state));
            }
        }
    }
}
```

---

## 実装のポイント

1. **StateContext**: empty な optional を持つだけ
2. **StateManager**: state_context_ メンバーを持つだけで、初期化処理不要
3. **InitState で emplace()**: ハードウェア初期化と同時に optional をインスタンス化
4. **責務分離**: 初期化がInitState という状態に紐付く
5. **シンプル**: StateManager は状態遷移に専念