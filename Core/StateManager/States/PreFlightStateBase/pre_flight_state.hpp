#ifndef PRE_FLIGHT_STATE_HPP
#define PRE_FLIGHT_STATE_HPP

#include "StateInterface/state_interface.hpp"
#include "StateInterface/state_id.hpp"
#include "StateInterface/state_result.hpp"


// 前方宣言
struct StateContext;


// 飛行前の準備状態を実装するクラス
class PreFlightState : public StateInterface {

    public:

        virtual ~PreFlightState() = default;


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

#endif // PRE_FLIGHT_STATE_HPP
