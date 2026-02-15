#include "Utility/ManeuverSequencer/maneuver_sequencer.hpp"

ManeuverSequencer::ManeuverSequencer() {
}

ManeuverSequencer::~ManeuverSequencer() {
}

bool ManeuverSequencer::isMissionComplete(uint32_t current_time_ms, const MissionBase* mission) const {

    if (mission == nullptr) {
        return false;
    }

    int num_frames = 0;
    const KeyFrame* key_frames = mission->getKeyFrames(num_frames);

    if (key_frames == nullptr || num_frames <= 0) {
        return false;
    }

    // 現在時刻が最後のキーフレームを超えている場合は完了
    if (current_time_ms > key_frames[num_frames - 1].time_ms) {
        return true;
    }

    return false;
}

float ManeuverSequencer::getTargetRoll(uint32_t current_time_ms, const MissionBase* mission) const {

    float roll = 0.0f;
    float pitch = 0.0f;
    float yaw = 0.0f;
    float altitude = 0.0f;

    getTargetValues(current_time_ms, mission, roll, pitch, yaw, altitude);

    return roll;
}

float ManeuverSequencer::getTargetPitch(uint32_t current_time_ms, const MissionBase* mission) const {

    float roll = 0.0f;
    float pitch = 0.0f;
    float yaw = 0.0f;
    float altitude = 0.0f;

    getTargetValues(current_time_ms, mission, roll, pitch, yaw, altitude);

    return pitch;
}

float ManeuverSequencer::getTargetYaw(uint32_t current_time_ms, const MissionBase* mission) const {

    float roll = 0.0f;
    float pitch = 0.0f;
    float yaw = 0.0f;
    float altitude = 0.0f;

    getTargetValues(current_time_ms, mission, roll, pitch, yaw, altitude);

    return yaw;
}

float ManeuverSequencer::getTargetAltitude(uint32_t current_time_ms, const MissionBase* mission) const {

    float roll = 0.0f;
    float pitch = 0.0f;
    float yaw = 0.0f;
    float altitude = 0.0f;

    getTargetValues(current_time_ms, mission, roll, pitch, yaw, altitude);

    return altitude;
}

bool ManeuverSequencer::getTargetValues(
    uint32_t current_time_ms,
    const MissionBase* mission,
    float& target_roll_deg,
    float& target_pitch_deg,
    float& target_yaw_deg,
    float& target_altitude_m
) const {

    if (mission == nullptr) {
        return false;
    }

    int num_frames = 0;
    const KeyFrame* key_frames = mission->getKeyFrames(num_frames);

    if (key_frames == nullptr || num_frames <= 0) {
        return false;
    }

    // 最初のフレームより前の場合
    if (current_time_ms <= key_frames[0].time_ms) {
        target_roll_deg = key_frames[0].roll_deg;
        target_pitch_deg = key_frames[0].pitch_deg;
        target_yaw_deg = key_frames[0].yaw_deg;
        target_altitude_m = key_frames[0].altitude_m;
        return true;
    }

    // 最後のフレームより後の場合
    if (current_time_ms >= key_frames[num_frames - 1].time_ms) {
        target_roll_deg = key_frames[num_frames - 1].roll_deg;
        target_pitch_deg = key_frames[num_frames - 1].pitch_deg;
        target_yaw_deg = key_frames[num_frames - 1].yaw_deg;
        target_altitude_m = key_frames[num_frames - 1].altitude_m;
        return true;
    }

    // 現在時刻の前後のフレームを探す
    int frame_index = 0;
    if (!findFrameIndices(current_time_ms, key_frames, num_frames, frame_index)) {
        return false;
    }

    // フレームAとフレームB（次のフレーム）を取得
    const KeyFrame& frame_a = key_frames[frame_index];
    const KeyFrame& frame_b = key_frames[frame_index + 1];

    // 線形補間で目標値を計算
    target_roll_deg = linearInterpolation(
        frame_a.time_ms,
        frame_a.roll_deg,
        frame_b.time_ms,
        frame_b.roll_deg,
        current_time_ms
    );

    target_pitch_deg = linearInterpolation(
        frame_a.time_ms,
        frame_a.pitch_deg,
        frame_b.time_ms,
        frame_b.pitch_deg,
        current_time_ms
    );

    target_yaw_deg = linearInterpolation(
        frame_a.time_ms,
        frame_a.yaw_deg,
        frame_b.time_ms,
        frame_b.yaw_deg,
        current_time_ms
    );

    target_altitude_m = linearInterpolation(
        frame_a.time_ms,
        frame_a.altitude_m,
        frame_b.time_ms,
        frame_b.altitude_m,
        current_time_ms
    );

    return true;
}

bool ManeuverSequencer::findFrameIndices(
    uint32_t current_time_ms,
    const KeyFrame* key_frames,
    int num_frames,
    int& frame_index
) const {

    if (key_frames == nullptr || num_frames <= 0) {
        return false;
    }

    // 線形探索で現在時刻の直前のフレームを探す
    for (int i = 0; i < num_frames - 1; i++) {
        if (current_time_ms >= key_frames[i].time_ms && current_time_ms <= key_frames[i + 1].time_ms) {
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
    uint32_t current_time_ms
) const {

    // 時間差が0の場合はvalue_aを返す
    if (time_b == time_a) {
        return value_a;
    }

    // 線形補間: value = value_a + (current_time_ms - time_a) * (value_b - value_a) / (time_b - time_a)
    float time_ratio = static_cast<float>(current_time_ms - time_a) / static_cast<float>(time_b - time_a);
    float interpolated_value = value_a + time_ratio * (value_b - value_a);

    return interpolated_value;
}
