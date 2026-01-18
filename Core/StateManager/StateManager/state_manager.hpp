#ifndef STATE_MANAGER_HPP
#define STATE_MANAGER_HPP

#include <memory>
#include <optional>
#include "StateInterface/state_interface.hpp"
#include "StateInterface/state_id.hpp"
#include "StateContext/context.hpp"
#include "LoopManager/loop_manager.hpp"


// 前方宣言
class IMUSensor;
class MagnetometerSensor;
class LiDARSensor;
class BarometricSensor;
class MotorDriver;
class PIDController;


// 状態管理クラス
class StateManager {

    public:

        // コンストラクタ（初期状態を設定）
        StateManager(StateID init_state_id);

        // デストラクタ
        virtual ~StateManager() = default;

        // メインループの更新処理
        void update();


    private:

        // StateContext をメンバーとして保持
        StateContext state_context_;

        // 現在の状態
        std::unique_ptr<StateInterface> current_state_;

        // ループ管理（10Hz = 100000us）
        LoopManager loop_manager_;

        // 状態遷移
        void changeState(std::unique_ptr<StateInterface> new_state);
};

#endif // STATE_MANAGER_HPP
