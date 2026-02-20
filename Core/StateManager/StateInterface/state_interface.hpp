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

        // 状態がアクティブな間に定期的に呼ばれる処理
        virtual StateResult update(StateContext& context) = 0;

        // 現在の状態IDを取得する
        virtual StateID getStateID() const = 0;

        // 状態が属する基底クラスのIDを取得する
        virtual StateBaseID getStateBaseID() const = 0;
};

#endif // STATE_INTERFACE_HPP
