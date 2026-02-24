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
class InitState : public InitStateBase {
    public:
        virtual ~InitState() = default;
        virtual StateID getStateID() const override;
    private:
        virtual ProcessStatus onUpdate(StateContext& context) override;
        virtual StateID evaluateNextState(StateContext& context) override;

        ProcessStatus initializeSensors(StateContext& context);
        ProcessStatus initializePWM(StateContext& context);
        ProcessStatus initializeAttitudeEstimation(StateContext& context);
        ProcessStatus initializeSBUS(StateContext& context);
};


// ============================================================================
// PreFlightStateBase States
// ============================================================================

// キャリブレーション状態を実装するクラス
class CalibrationState : public PreFlightStateBase {
    public:
        virtual ~CalibrationState() = default;
        virtual StateID getStateID() const override;
    private:
        virtual ProcessStatus onUpdate(StateContext& context) override;
        virtual StateID evaluateNextState(StateContext& context) override;
};

// 飛行前の準備状態を実装するクラス
class PreFlightState : public PreFlightStateBase {
    public:
        virtual ~PreFlightState() = default;
        virtual StateID getStateID() const override;
    private:
        virtual ProcessStatus onUpdate(StateContext& context) override;
        virtual StateID evaluateNextState(StateContext& context) override;
        int loop_count = 0;
};

// サーボ・モーターテスト状態を実装するクラス
class ServoMotorTestState : public PreFlightStateBase {
    public:
        virtual ~ServoMotorTestState() = default;
        virtual StateID getStateID() const override;
    private:
        virtual ProcessStatus onUpdate(StateContext& context) override;
        virtual StateID evaluateNextState(StateContext& context) override;
        int loop_count = 0;
};

// センサーテスト状態を実装するクラス
class SensorTestState : public PreFlightStateBase {
    public:
        virtual ~SensorTestState() = default;
        virtual StateID getStateID() const override;
    private:
        virtual ProcessStatus onUpdate(StateContext& context) override;
        virtual StateID evaluateNextState(StateContext& context) override;
        int loop_count = 0;
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
        virtual ProcessStatus onUpdate(StateContext& context) override;
        virtual StateID evaluateNextState(StateContext& context) override;
};

// 水平飛行状態を実装するクラス
class LevelFlightState : public ManualFlightStateBase {
    public:
        virtual ~LevelFlightState() = default;
        virtual StateID getStateID() const override;
    private:
        virtual ProcessStatus onUpdate(StateContext& context) override;
        virtual StateID evaluateNextState(StateContext& context) override;
};

// レベルターン状態を実装するクラス
class LevelTurnState : public ManualFlightStateBase {
    public:
        virtual ~LevelTurnState() = default;
        virtual StateID getStateID() const override;
    private:
        virtual ProcessStatus onUpdate(StateContext& context) override;
        virtual StateID evaluateNextState(StateContext& context) override;
};

// クライミングターン状態を実装するクラス
class ClimbingTurnState : public ManualFlightStateBase {
    public:
        virtual ~ClimbingTurnState() = default;
        virtual StateID getStateID() const override;
    private:
        virtual ProcessStatus onUpdate(StateContext& context) override;
        virtual StateID evaluateNextState(StateContext& context) override;
};

// フュージュエイト状態を実装するクラス
class FugueEightState : public ManualFlightStateBase {
    public:
        virtual ~FugueEightState() = default;
        virtual StateID getStateID() const override;
    private:
        virtual ProcessStatus onUpdate(StateContext& context) override;
        virtual StateID evaluateNextState(StateContext& context) override;
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
        virtual ProcessStatus onUpdate(StateContext& context) override;
        virtual StateID evaluateNextState(StateContext& context) override;
};

// 自動離陸状態を実装するクラス
class AutoTakeoffState : public AutoFlightStateBase {
    public:
        virtual ~AutoTakeoffState() = default;
        virtual StateID getStateID() const override;
    private:
        virtual ProcessStatus onUpdate(StateContext& context) override;
        virtual StateID evaluateNextState(StateContext& context) override;
};

// 自動飛行状態を実装するクラス
class AutoFlightState : public AutoFlightStateBase {
    public:
        virtual ~AutoFlightState() = default;
        virtual StateID getStateID() const override;
    private:
        virtual ProcessStatus onUpdate(StateContext& context) override;
        virtual StateID evaluateNextState(StateContext& context) override;
};

// 自動着陸状態を実装するクラス
class AutoLandingState : public AutoFlightStateBase {
    public:
        virtual ~AutoLandingState() = default;
        virtual StateID getStateID() const override;
    private:
        virtual ProcessStatus onUpdate(StateContext& context) override;
        virtual StateID evaluateNextState(StateContext& context) override;
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
        virtual ProcessStatus onUpdate(StateContext& context) override;
        virtual StateID evaluateNextState(StateContext& context) override;
};

// 緊急状態を実装するクラス
class EmergencyState : public PostFlightStateBase {
    public:
        virtual ~EmergencyState() = default;
        virtual StateID getStateID() const override;
    private:
        virtual ProcessStatus onUpdate(StateContext& context) override;
        virtual StateID evaluateNextState(StateContext& context) override;
};

#endif // STATE_HEADERS_HPP
