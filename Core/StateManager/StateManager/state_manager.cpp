#include "StateManager/state_manager.hpp"
#include "StateFactory/state_factory.hpp"


// コンストラクタ
StateManager::StateManager(StateID init_state_id) : loop_manager_(100000) {

    // 初期状態を生成
    current_state_ = StateFactory::createState(init_state_id);

    // 初期状態のenter関数を呼ぶ
    if (current_state_) 
    {
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
