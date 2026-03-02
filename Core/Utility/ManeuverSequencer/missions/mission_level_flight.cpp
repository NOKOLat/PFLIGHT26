#include "missions.hpp"

MissionLevelFlight::MissionLevelFlight() {

    // キーフレーム定義
    // time_ms, roll_deg, pitch_deg, yaw_deg, altitude_m
    // 注：altitude_m は ATTITUDE チャンネルのみなので使用されない
    key_frames[0] = {0,     0.0f,  0.0f,  0.0f, 0.0f};      // 開始: すべて0度
    key_frames[1] = {15000, 0.0f,  0.0f,  0.0f, 0.0f};      // 15秒後: すべて0度（水平飛行継続）
}

const KeyFrame* MissionLevelFlight::getKeyFrames(int& num_frames) const {

    num_frames = NUM_KEY_FRAMES;
    return key_frames;
}

uint8_t MissionLevelFlight::getActiveChannels() const {

    // 高度制御を使用しない（ATTITUDE: ROLL | PITCH | YAW のみ）
    return ChannelFlags::ATTITUDE;
}
