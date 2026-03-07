#ifndef CASCADE_PID_CONTROLLER_HPP
#define CASCADE_PID_CONTROLLER_HPP

#include <cstdint>
#include <cstddef>
#include "1DoF_PID/PID.h"

// Forward declaration
struct StateContext;
struct AttitudeState;

/**
 * @class CascadePIDController
 * @brief カスケード制御用PIDコントローラ
 * 
 * 外側ループ（角度制御）と内側ループ（角速度制御）を独立して計算できます。
 * PIDインスタンスを内部管理します。
 * 
 * 使用例：
 *   - 外側: 目標角度 → 目標角速度
 *   - 内側: 目標角速度 → 制御出力
 */
class CascadePIDController {

public:
    // ========== コンストラクタ・デストラクタ ==========
    CascadePIDController();
    ~CascadePIDController() = default;

    // ========== 初期化 ==========
    /**
     * @brief PIDコントローラーを初期化（pid_config.hppから設定値を読み込む）
     * @param dt サンプリング時間 [秒]
     * @return 初期化成功時true
     */
    bool initialize(float dt);

    // ========== ループ別計算（メイン） ==========
    /**
     * @brief 外側ループ（角度制御）: 目標角度 → 目標角速度
     * @param axis_index 軸インデックス (0=pitch, 1=roll, 2=yaw)
     * @param target_angle 目標角度 [deg]
     * @param current_angle 現在角度 [deg]
     * @return 目標角速度 [deg/s]
     */
    float computeOuterLoop(int axis_index, float target_angle, float current_angle);

    /**
     * @brief 内側ループ（角速度制御）: 目標角速度 → 制御出力
     * @param axis_index 軸インデックス (0=pitch, 1=roll, 2=yaw)
     * @param target_rate 目標角速度 [deg/s]
     * @param current_rate 現在角速度 [deg/s]
     * @return 制御出力（PWM値等）[%]
     */
    float computeInnerLoop(int axis_index, float target_rate, float current_rate);

    // ========== リセット ==========
    /**
     * @brief すべての軸のPIDをリセット（積分値をクリア）
     */
    void reset();

    /**
     * @brief 指定軸のPIDをリセット
     * @param axis_index 対象軸 (0=pitch, 1=roll, 2=yaw)
     */
    void resetAxis(int axis_index);

    // ========== ゲイン動的設定 ==========
    /**
     * @brief 外側ループ（角度制御）のゲインを設定
     * @param axis_index 対象軸 (0=pitch, 1=roll, 2=yaw)
     * @param kp 比例ゲイン
     * @param ki 積分ゲイン
     * @param kd 微分ゲイン
     */
    void setOuterGain(int axis_index, float kp, float ki, float kd);

    /**
     * @brief 内側ループ（角速度制御）のゲインを設定
     * @param axis_index 対象軸 (0=pitch, 1=roll, 2=yaw)
     * @param kp 比例ゲイン
     * @param ki 積分ゲイン
     * @param kd 微分ゲイン
     */
    void setInnerGain(int axis_index, float kp, float ki, float kd);

private:
    // ========== 軸マッピング ==========
    enum AxisIndex {
        PITCH = 0,
        ROLL = 1,
        YAW = 2,
        NUM_AXES = 3
    };

    // ========== 内部状態 ==========
    PID angle_pid[NUM_AXES];      // 外側ループPID [pitch, roll, yaw]
    PID rate_pid[NUM_AXES];       // 内側ループPID [pitch, roll, yaw]
    bool is_initialized = false;
};

#endif // CASCADE_PID_CONTROLLER_HPP
