#include "missions.hpp"

MissionLevelTurn::MissionLevelTurn() {

    // キーフレーム定義
    // time_ms, roll_deg, pitch_deg, yaw_deg, altitude_m
    // 注：altitude_m は ATTITUDE チャンネルのみなので使用されない
    key_frames[0] = {0,     15.0f,  0.0f,  0.0f,  0.0f};     
    key_frames[1] = {50000, 15.0f, 0.0f,  0.0f, 0.0f};   
}

const KeyFrame* MissionLevelTurn::getKeyFrames(int& num_frames) const {

    num_frames = NUM_KEY_FRAMES;
    return key_frames;
}

uint8_t MissionLevelTurn::getActiveChannels() const {

    // 高度制御を使用しない（ATTITUDE: ROLL | PITCH | YAW のみ）
    return ChannelFlags::ATTITUDE;
}
