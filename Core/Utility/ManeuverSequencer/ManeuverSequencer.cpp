#include "ManeuverSequencer.hpp"

// ===============================================================
// ライフサイクルメソッド実装
// ===============================================================

bool ManeuverSequencer::initialize(const ManeuverKeyframe* keyframes,
                                   uint32_t keyframe_count,
                                   uint32_t duration_ms)
{
    // 入力の妥当性チェック
    if (!keyframes || keyframe_count == 0) {
        return false;
    }

    keyframes_ = keyframes;
    keyframe_count_ = keyframe_count;
    duration_ms_ = duration_ms;
    elapsed_time_ms_ = 0;

    // 初期セットポイントを計算
    computeSetpoint();

    return true;
}

void ManeuverSequencer::update(uint32_t current_time_ms)
{
    if (!keyframes_) {
        return;
    }

    // 経過時間をクリップ（終了時刻を超えない）
    elapsed_time_ms_ = (current_time_ms > duration_ms_) ? duration_ms_ : current_time_ms;

    // 現在のセットポイントを計算
    computeSetpoint();
}

// ===============================================================
// プライベートメソッド実装
// ===============================================================

void ManeuverSequencer::computeSetpoint()
{
    if (!keyframes_ || keyframe_count_ == 0) {
        current_setpoint_ = ManeuverSetpoint();
        return;
    }

    // 指定時刻に対応するキーフレームペアを探す
    uint32_t idx = findKeyframeIndex(elapsed_time_ms_);

    // 最後のキーフレームの場合はそのまま返す
    if (idx >= keyframe_count_ - 1) {
        const ManeuverKeyframe& last_frame = keyframes_[keyframe_count_ - 1];
        current_setpoint_ = ManeuverSetpoint{
            last_frame.roll_target,
            last_frame.pitch_target,
            last_frame.yaw_target,
            last_frame.thrust_target,
            0.0f, 0.0f, 0.0f
        };
        return;
    }

    // キーフレーム間を線形補間
    const ManeuverKeyframe& frame_a = keyframes_[idx];
    const ManeuverKeyframe& frame_b = keyframes_[idx + 1];
    current_setpoint_ = lerpKeyframe(frame_a, frame_b, elapsed_time_ms_);
}

uint32_t ManeuverSequencer::findKeyframeIndex(uint32_t elapsed_time_ms) const
{
    // 最初のキーフレームより前の場合
    if (elapsed_time_ms < keyframes_[0].time_ms) {
        return 0;
    }

    // 最後のキーフレーム以降の場合
    if (elapsed_time_ms >= keyframes_[keyframe_count_ - 1].time_ms) {
        return keyframe_count_ - 1;
    }

    // 二分探索（または線形探索）で該当するキーフレームを探す
    // 小さいキーフレーム数の場合は線形探索で十分
    for (uint32_t i = 0; i < keyframe_count_ - 1; ++i) {
        if (keyframes_[i].time_ms <= elapsed_time_ms &&
            elapsed_time_ms < keyframes_[i + 1].time_ms) {
            return i;
        }
    }

    return keyframe_count_ - 1;
}

void ManeuverSequencer::reset()
{
    elapsed_time_ms_ = 0;
    computeSetpoint();
}
