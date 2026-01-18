#ifndef CLIMBING_TURN_STATE_HPP
#define CLIMBING_TURN_STATE_HPP

#include "StateBase/manual_flight_state_base.hpp"
#include "StateInterface/state_id.hpp"
#include "StateInterface/state_result.hpp"


// 前方宣言
struct StateContext;


// クライミングターン状態を実装するクラス
class ClimbingTurnState : public ManualFlightStateBase {

    public:

        virtual ~ClimbingTurnState() = default;


        // 現在の状態IDを取得する
        virtual StateID getStateID() const override;


    private:

        // 派生クラス固有の初期化処理
        virtual void onEnter(StateContext& context) override;

        // 派生クラス固有のクリーンアップ処理
        virtual void onExit(StateContext& context) override;

        // 派生クラス固有の更新処理
        virtual StateResult onUpdate(StateContext& context) override;
};

#endif // CLIMBING_TURN_STATE_HPP
