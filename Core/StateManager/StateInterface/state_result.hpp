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
