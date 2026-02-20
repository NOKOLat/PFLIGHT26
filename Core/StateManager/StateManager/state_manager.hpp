#ifndef STATE_MANAGER_HPP
#define STATE_MANAGER_HPP

#include <memory>
#include <optional>
#include "StateInterface/state_interface.hpp"
#include "StateInterface/state_id.hpp"
#include "StateContext/context.hpp"


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
        StateManager(StateID init_state_id, uint32_t loop_time_us);

        // デストラクタ
        virtual ~StateManager() = default;

        // コンテキストの取得
        StateContext& getContext();

        // メインループの更新処理
        void update();


    private:

        // 初期化処理
        void init();

        // 状態遷移（StateIDを受け取りFactoryで状態を生成）
        void changeState(StateID state_id);

        // SBUSデータの更新・フェイルセーフ判定
        void updateSBUS();

        // 初期状態ID
        StateID init_state_id_;

        // StateContext をメンバーとして保持
        StateContext state_context_;

        // 初回実行フラグ
        bool is_first_execution_ = true;

        // 現在の状態
        std::unique_ptr<StateInterface> current_state_;
};

#endif // STATE_MANAGER_HPP
