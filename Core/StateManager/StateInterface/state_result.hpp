#ifndef STATE_RESULT_HPP
#define STATE_RESULT_HPP

#include "state_id.hpp"


// 処理の成否を表すenum
enum class ProcessStatus : bool {
    SUCCESS = true,
    FAILURE = false
};

// 状態遷移の有無を表すenum
enum class TransitionFlag : bool {
    SHOULD_TRANSITION = true,
    NO_TRANSITION = false
};


// 状態処理の結果を表す構造体
struct StateResult {

    // 処理の成否
    ProcessStatus success;

    // 状態遷移の有無
    TransitionFlag should_transition;

    // 次の状態ID（should_transition == true の時のみ有効）
    StateID next_state_id;

    // エラーメッセージ（デバッグ用）
    const char* error_message = nullptr;
};

#endif // STATE_RESULT_HPP
