#include "missions/missions.hpp"

MissionTestSimple::MissionTestSimple() {

    // キーフレームデータの初期化
    // time_ms, roll_deg, pitch_deg, yaw_deg, altitude_m
    key_frames[0] = {0,    0.0f,  0.0f,  0.0f, 0.0f};
    key_frames[1] = {1000, 20.0f, 5.0f, 10.0f, 10.0f};
    key_frames[2] = {2000, 40.0f, 10.0f, 20.0f, 20.0f};
    key_frames[3] = {3000, 0.0f,  0.0f,  0.0f, 0.0f};
}

MissionTestSimple::~MissionTestSimple() {
}

const KeyFrame* MissionTestSimple::getKeyFrames(int& num_frames) const {

    num_frames = NUM_KEY_FRAMES;
    return key_frames;
}
