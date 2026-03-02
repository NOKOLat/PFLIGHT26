#include "missions.hpp"

MissionTest::MissionTest() {

    // キーフレームデータの初期化
    // time_ms, roll_deg, pitch_deg, yaw_deg, altitude_m
    key_frames[0] = {0,    0.0f,  0.0f,  0.0f, 0.0f};
    key_frames[1] = {1000, 10.0f, 5.0f,  5.0f, 5.0f};
    key_frames[2] = {2000, 20.0f, 10.0f, 10.0f, 10.0f};
    key_frames[3] = {3000, 0.0f,  0.0f,  0.0f, 0.0f};
}


const KeyFrame* MissionTest::getKeyFrames(int& num_frames) const {

    num_frames = NUM_KEY_FRAMES;
    return key_frames;
}

uint8_t MissionTest::getActiveChannels() const {

    return ChannelFlags::ALL;
}
