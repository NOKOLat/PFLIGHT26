#ifndef STATE_HEADERS_HPP
#define STATE_HEADERS_HPP

#include "StateInterface/state_interface.hpp"
#include "StateInterface/state_id.hpp"
#include "StateInterface/state_result.hpp"
#include "StateBase/state_base.hpp"

// 前方宣言
struct StateContext;

// ============================================================================
// InitStateBase States
// ============================================================================

// 初期化状態を実装するクラス
class InitState : public StateInterface {
    public:
        virtual ~InitState() = default;
        virtual void enter(StateContext& context) override;
        virtual StateResult update(StateContext& context) override;
        virtual void exit(StateContext& context) override;
        virtual StateID getStateID() const override;
        virtual StateBaseID getStateBaseID() const override;
};


// ============================================================================
// PreFlightStateBase States
// ============================================================================

// キャリブレーション状態を実装するクラス
class CalibrationState : public StateInterface {
    public:
        virtual ~CalibrationState() = default;
        virtual void enter(StateContext& context) override;
        virtual StateResult update(StateContext& context) override;
        virtual void exit(StateContext& context) override;
        virtual StateID getStateID() const override;
        virtual StateBaseID getStateBaseID() const override;
};

// 飛行前の準備状態を実装するクラス
class PreFlightState : public StateInterface {
    public:
        virtual ~PreFlightState() = default;
        virtual void enter(StateContext& context) override;
        virtual StateResult update(StateContext& context) override;
        virtual void exit(StateContext& context) override;
        virtual StateID getStateID() const override;
        virtual StateBaseID getStateBaseID() const override;
};


// ============================================================================
// ManualFlightStateBase States
// ============================================================================

// 手動飛行状態を実装するクラス
class ManualFlightState : public ManualFlightStateBase {
    public:
        virtual ~ManualFlightState() = default;
        virtual StateID getStateID() const override;
    private:
        virtual void onEnter(StateContext& context) override;
        virtual void onExit(StateContext& context) override;
        virtual StateResult onUpdate(StateContext& context) override;
};

// レベルターン状態を実装するクラス
class LevelTurnState : public ManualFlightStateBase {
    public:
        virtual ~LevelTurnState() = default;
        virtual StateID getStateID() const override;
    private:
        virtual void onEnter(StateContext& context) override;
        virtual void onExit(StateContext& context) override;
        virtual StateResult onUpdate(StateContext& context) override;
};

// クライミングターン状態を実装するクラス
class ClimbingTurnState : public ManualFlightStateBase {
    public:
        virtual ~ClimbingTurnState() = default;
        virtual StateID getStateID() const override;
    private:
        virtual void onEnter(StateContext& context) override;
        virtual void onExit(StateContext& context) override;
        virtual StateResult onUpdate(StateContext& context) override;
};

// フュージュエイト状態を実装するクラス
class FugueEightState : public ManualFlightStateBase {
    public:
        virtual ~FugueEightState() = default;
        virtual StateID getStateID() const override;
    private:
        virtual void onEnter(StateContext& context) override;
        virtual void onExit(StateContext& context) override;
        virtual StateResult onUpdate(StateContext& context) override;
};


// ============================================================================
// AutoFlightStateBase States
// ============================================================================

// 自動飛行前準備状態を実装するクラス
class PreAutoFlightState : public AutoFlightStateBase {
    public:
        virtual ~PreAutoFlightState() = default;
        virtual StateID getStateID() const override;
    private:
        virtual void onEnter(StateContext& context) override;
        virtual void onExit(StateContext& context) override;
        virtual StateResult onUpdate(StateContext& context) override;
};

// 自動離陸状態を実装するクラス
class AutoTakeoffState : public AutoFlightStateBase {
    public:
        virtual ~AutoTakeoffState() = default;
        virtual StateID getStateID() const override;
    private:
        virtual void onEnter(StateContext& context) override;
        virtual void onExit(StateContext& context) override;
        virtual StateResult onUpdate(StateContext& context) override;
};

// 自動飛行状態を実装するクラス
class AutoFlightState : public AutoFlightStateBase {
    public:
        virtual ~AutoFlightState() = default;
        virtual StateID getStateID() const override;
    private:
        virtual void onEnter(StateContext& context) override;
        virtual void onExit(StateContext& context) override;
        virtual StateResult onUpdate(StateContext& context) override;
};

// 自動着陸状態を実装するクラス
class AutoLandingState : public AutoFlightStateBase {
    public:
        virtual ~AutoLandingState() = default;
        virtual StateID getStateID() const override;
    private:
        virtual void onEnter(StateContext& context) override;
        virtual void onExit(StateContext& context) override;
        virtual StateResult onUpdate(StateContext& context) override;
};


// ============================================================================
// PostFlightStateBase States
// ============================================================================

// 飛行後の状態を実装するクラス
class PostFlightState : public PostFlightStateBase {
    public:
        virtual ~PostFlightState() = default;
        virtual StateID getStateID() const override;
    private:
        virtual void onEnter(StateContext& context) override;
        virtual void onExit(StateContext& context) override;
        virtual StateResult onUpdate(StateContext& context) override;
};

// 緊急状態を実装するクラス
class EmergencyState : public PostFlightStateBase {
    public:
        virtual ~EmergencyState() = default;
        virtual StateID getStateID() const override;
    private:
        virtual void onEnter(StateContext& context) override;
        virtual void onExit(StateContext& context) override;
        virtual StateResult onUpdate(StateContext& context) override;
};

#endif // STATE_HEADERS_HPP
