#ifndef LEVEL_TURN_STATE_HPP
#define LEVEL_TURN_STATE_HPP

#include "StateBase/manual_flight_state_base.hpp"
#include "StateInterface/state_id.hpp"
#include "StateInterface/state_result.hpp"


// 前方宣言
struct StateContext;


// レベルターン状態を実装するクラス
class LevelTurnState : public ManualFlightStateBase {

    public:

        virtual ~LevelTurnState() = default;


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

#endif // LEVEL_TURN_STATE_HPP
