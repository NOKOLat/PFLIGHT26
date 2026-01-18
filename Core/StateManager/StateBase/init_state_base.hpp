#ifndef INIT_STATE_BASE_HPP
#define INIT_STATE_BASE_HPP

#include "StateInterface/state_interface.hpp"
#include "StateInterface/state_id.hpp"
#include "StateInterface/state_result.hpp"


// 前方宣言
struct StateContext;


// 初期化状態の共通処理を実装する基底クラス
class InitStateBase : public StateInterface {

    protected:

        // 派生クラスで実装すべき処理
        // 派生クラス固有の初期化処理
        virtual void onEnter(StateContext& context) = 0;

        // 派生クラス固有のクリーンアップ処理
        virtual void onExit(StateContext& context) = 0;

        // 派生クラス固有の更新処理
        virtual StateResult onUpdate(StateContext& context) = 0;


    public:

        virtual ~InitStateBase() = default;


        // 状態に入る際に呼ばれる共通初期化処理
        virtual void enter(StateContext& context) override;


        // 状態がアクティブな間に定期的に呼ばれる共通処理
        virtual StateResult update(StateContext& context) override;


        // 状態から退出する際に呼ばれる共通クリーンアップ処理
        virtual void exit(StateContext& context) override;


        // 状態が属する基底クラスのIDを取得する
        virtual StateBaseID getStateBaseID() const override;
};

#endif // INIT_STATE_BASE_HPP
