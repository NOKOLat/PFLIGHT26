#include "maneuver_sequencer.hpp"
#include "Missions/missions.hpp"
#include "stm32f7xx_hal.h"
#include <cmath>

ManeuverSequencer::ManeuverSequencer() {

}

ManeuverSequencer::~ManeuverSequencer() {

}

void ManeuverSequencer::startMission(const MissionBase* mission) {

    if (mission == nullptr) {
        return;
    }

    current_mission_ = mission;
    mission_start_ms_ = HAL_GetTick();
    mission_started_ = true;
}

bool ManeuverSequencer::isMissionComplete() const {

    if (current_mission_ == nullptr || !mission_started_) {
        return false;
    }

    int num_frames = 0;
    const KeyFrame* key_frames = current_mission_->getKeyFrames(num_frames);

    if (key_frames == nullptr || num_frames <= 0) {
        return false;
    }

    // 現在時刻が最後のキーフレームを超えている場合は完了
    uint32_t current_ms = HAL_GetTick();
    uint32_t elapsed_ms = current_ms - mission_start_ms_;

    if (elapsed_ms > key_frames[num_frames - 1].time_ms) {
        return true;
    }

    return false;
}

bool ManeuverSequencer::getTargetValues(
    float& roll_deg,
    float& pitch_deg,
    float& yaw_deg,
    float& altitude_m
) const {

    if (current_mission_ == nullptr || !mission_started_) {
        return false;
    }

    int num_frames = 0;
    const KeyFrame* key_frames = current_mission_->getKeyFrames(num_frames);

    if (key_frames == nullptr || num_frames <= 0) {
        return false;
    }

    // 経過時間を計算
    uint32_t current_ms = HAL_GetTick();
    uint32_t elapsed_ms = current_ms - mission_start_ms_;

    // 最初のフレームより前の場合
    if (elapsed_ms <= key_frames[0].time_ms) {
        roll_deg = key_frames[0].roll_deg;
        pitch_deg = key_frames[0].pitch_deg;
        yaw_deg = key_frames[0].yaw_deg;
        altitude_m = key_frames[0].altitude_m;
        return true;
    }

    // 最後のフレームより後の場合
    if (elapsed_ms >= key_frames[num_frames - 1].time_ms) {
        roll_deg = key_frames[num_frames - 1].roll_deg;
        pitch_deg = key_frames[num_frames - 1].pitch_deg;
        yaw_deg = key_frames[num_frames - 1].yaw_deg;
        altitude_m = key_frames[num_frames - 1].altitude_m;
        return true;
    }

    // 現在時刻の前後のフレームを探す
    int frame_index = 0;
    if (!findFrameIndices(elapsed_ms, key_frames, num_frames, frame_index)) {
        return false;
    }

    // フレーム A とフレーム B（次のフレーム）を取得
    const KeyFrame& frame_a = key_frames[frame_index];
    const KeyFrame& frame_b = key_frames[frame_index + 1];

    // 線形補間で目標値を計算
    roll_deg = linearInterpolation(
        frame_a.time_ms,
        frame_a.roll_deg,
        frame_b.time_ms,
        frame_b.roll_deg,
        elapsed_ms
    );

    pitch_deg = linearInterpolation(
        frame_a.time_ms,
        frame_a.pitch_deg,
        frame_b.time_ms,
        frame_b.pitch_deg,
        elapsed_ms
    );

    yaw_deg = linearInterpolation(
        frame_a.time_ms,
        frame_a.yaw_deg,
        frame_b.time_ms,
        frame_b.yaw_deg,
        elapsed_ms
    );

    altitude_m = linearInterpolation(
        frame_a.time_ms,
        frame_a.altitude_m,
        frame_b.time_ms,
        frame_b.altitude_m,
        elapsed_ms
    );

    return true;
}

uint8_t ManeuverSequencer::getActiveChannels() const {

    if (current_mission_ == nullptr || !mission_started_) {
        return 0;
    }

    return current_mission_->getActiveChannels();
}

bool ManeuverSequencer::findFrameIndices(
    uint32_t current_ms,
    const KeyFrame* key_frames,
    int num_frames,
    int& frame_index
) const {

    if (key_frames == nullptr || num_frames <= 0) {
        return false;
    }

    // 線形探索で現在時刻の直前のフレームを探す
    for (int i = 0; i < num_frames - 1; i++) {

        if (current_ms >= key_frames[i].time_ms && current_ms <= key_frames[i + 1].time_ms) {
            frame_index = i;
            return true;
        }
    }

    return false;
}

float ManeuverSequencer::linearInterpolation(
    uint32_t time_a,
    float value_a,
    uint32_t time_b,
    float value_b,
    uint32_t current_ms
) const {

    // 時間差が 0 の場合は value_a を返す
    if (time_b == time_a) {
        return value_a;
    }

    // 線形補間: value = value_a + (current_ms - time_a) * (value_b - value_a) / (time_b - time_a)
    float time_ratio = static_cast<float>(current_ms - time_a) / static_cast<float>(time_b - time_a);
    float interpolated_value = value_a + time_ratio * (value_b - value_a);

    return interpolated_value;
}
