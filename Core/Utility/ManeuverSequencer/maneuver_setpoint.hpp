#ifndef MANEUVER_SETPOINT_HPP
#define MANEUVER_SETPOINT_HPP

#include <cstdint>
#include <cmath>

// ===============================================================
// 制御目標値（セットポイント）
//
// 機動シーケンサーが毎フレーム出力する目標値
// ===============================================================

struct ManeuverSetpoint {
    // 姿勢目標値
    float roll_target = 0.0f;         // 目標ロール角 [deg]
    float pitch_target = 0.0f;        // 目標ピッチ角 [deg]
    float yaw_target = 0.0f;          // 目標ヨー角 [deg]

    // スロットル出力
    float thrust_target = 0.0f;       // 目標スロットル [0.0 - 100.0] %

    // オプション: より高度な制御向け
    float roll_rate_target = 0.0f;    // 目標ロール角速度 [deg/s]
    float pitch_rate_target = 0.0f;   // 目標ピッチ角速度 [deg/s]
    float yaw_rate_target = 0.0f;     // 目標ヨー角速度 [deg/s]
};

// ===============================================================
// キーフレーム（時刻付きセットポイント）
//
// ミッションデータは複数のキーフレームの配列として保持される
// ManeuverSequencer がキーフレーム間を線形補間する
// ===============================================================

struct ManeuverKeyframe {
    uint32_t time_ms = 0;             // キーフレームの時刻 [ms]
    
    // 姿勢目標値
    float roll_target = 0.0f;         // 目標ロール角 [deg]
    float pitch_target = 0.0f;        // 目標ピッチ角 [deg]
    float yaw_target = 0.0f;          // 目標ヨー角 [deg]

    // スロットル出力
    float thrust_target = 0.0f;       // 目標スロットル [0.0 - 100.0] %
    
};

// ===============================================================
// ユーティリティ関数：キーフレーム間の線形補間
// ===============================================================

inline ManeuverSetpoint lerpKeyframe(
    const ManeuverKeyframe& frame_a,
    const ManeuverKeyframe& frame_b,
    uint32_t current_time_ms)
{
    // フレーム_aが次の時刻が同じ場合は frame_a を返す
    if (frame_a.time_ms == frame_b.time_ms) {
        return ManeuverSetpoint{
            frame_a.roll_target,
            frame_a.pitch_target,
            frame_a.yaw_target,
            frame_a.thrust_target,
            0.0f, 0.0f, 0.0f
        };
    }

    // 補間係数 t を計算 [0.0 - 1.0]
    float t = static_cast<float>(current_time_ms - frame_a.time_ms) /
              static_cast<float>(frame_b.time_ms - frame_a.time_ms);
    t = (t < 0.0f) ? 0.0f : (t > 1.0f) ? 1.0f : t;

    // 線形補間
    return ManeuverSetpoint{
        frame_a.roll_target + t * (frame_b.roll_target - frame_a.roll_target),
        frame_a.pitch_target + t * (frame_b.pitch_target - frame_a.pitch_target),
        frame_a.yaw_target + t * (frame_b.yaw_target - frame_a.yaw_target),
        frame_a.thrust_target + t * (frame_b.thrust_target - frame_a.thrust_target),
        0.0f, 0.0f, 0.0f
    };
}

#endif // MANEUVER_SETPOINT_HPP
