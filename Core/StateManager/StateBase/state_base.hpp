#ifndef STATE_BASE_HPP
#define STATE_BASE_HPP

#include "StateInterface/state_interface.hpp"
#include "StateInterface/state_id.hpp"
#include "StateInterface/state_result.hpp"


// 前方宣言
struct StateContext;


// ===== InitStateBase =====
// 初期化状態の共通処理を実装する基底クラス
class InitStateBase : public StateInterface {

    protected:

        // 派生クラスで実装すべき処理
        virtual void onEnter(StateContext& context) = 0;
        virtual void onExit(StateContext& context) = 0;
        virtual StateResult onUpdate(StateContext& context) = 0;

    public:

        virtual ~InitStateBase() = default;

        virtual void enter(StateContext& context) override;
        virtual StateResult update(StateContext& context) override;
        virtual void exit(StateContext& context) override;
        virtual StateBaseID getStateBaseID() const override;
};


// ===== PreFlightStateBase =====
// 飛行前の共通処理を実装する基底クラス
class PreFlightStateBase : public StateInterface {

    protected:

        virtual void onEnter(StateContext& context) = 0;
        virtual void onExit(StateContext& context) = 0;
        virtual StateResult onUpdate(StateContext& context) = 0;

    public:

        virtual ~PreFlightStateBase() = default;

        virtual void enter(StateContext& context) override;
        virtual StateResult update(StateContext& context) override;
        virtual void exit(StateContext& context) override;
        virtual StateBaseID getStateBaseID() const override;
};


// ===== ManualFlightStateBase =====
// 手動飛行の共通処理を実装する基底クラス
class ManualFlightStateBase : public StateInterface {

    protected:

        virtual void onEnter(StateContext& context) = 0;
        virtual void onExit(StateContext& context) = 0;
        virtual StateResult onUpdate(StateContext& context) = 0;

    public:

        virtual ~ManualFlightStateBase() = default;

        virtual void enter(StateContext& context) override;
        virtual StateResult update(StateContext& context) override;
        virtual void exit(StateContext& context) override;
        virtual StateBaseID getStateBaseID() const override;
};


// ===== AutoFlightStateBase =====
// 自動飛行の共通処理を実装する基底クラス
class AutoFlightStateBase : public StateInterface {

    protected:

        virtual void onEnter(StateContext& context) = 0;
        virtual void onExit(StateContext& context) = 0;
        virtual StateResult onUpdate(StateContext& context) = 0;

    public:

        virtual ~AutoFlightStateBase() = default;

        virtual void enter(StateContext& context) override;
        virtual StateResult update(StateContext& context) override;
        virtual void exit(StateContext& context) override;
        virtual StateBaseID getStateBaseID() const override;
};


// ===== PostFlightStateBase =====
// 飛行後の共通処理を実装する基底クラス
class PostFlightStateBase : public StateInterface {

    protected:

        virtual void onEnter(StateContext& context) = 0;
        virtual void onExit(StateContext& context) = 0;
        virtual StateResult onUpdate(StateContext& context) = 0;

    public:

        virtual ~PostFlightStateBase() = default;

        virtual void enter(StateContext& context) override;
        virtual StateResult update(StateContext& context) override;
        virtual void exit(StateContext& context) override;
        virtual StateBaseID getStateBaseID() const override;
};

#endif // STATE_BASE_HPP
