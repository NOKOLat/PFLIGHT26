#include "missions/missions.hpp"

MissionTestComplex::MissionTestComplex() {

    // キーフレームデータの初期化
    // 離陸: 0ms, 目標値0
    // 上昇と前進: 2000ms, ロール15deg, ピッチ10deg, ヨー0deg, 高度15m
    // 左旋回: 4000ms, ロール-25deg, ピッチ5deg, ヨー45deg, 高度20m
    // 高度調整: 6000ms, ロール10deg, ピッチ-5deg, ヨー90deg, 高度25m
    // 右旋回: 8000ms, ロール-20deg, ピッチ0deg, ヨー135deg, 高度20m
    // 着陸: 10000ms, 目標値0
    key_frames[0] = {0,    0.0f,   0.0f,   0.0f,  0.0f};
    key_frames[1] = {2000, 15.0f,  10.0f,  0.0f,  15.0f};
    key_frames[2] = {4000, -25.0f, 5.0f,   45.0f, 20.0f};
    key_frames[3] = {6000, 10.0f,  -5.0f,  90.0f, 25.0f};
    key_frames[4] = {8000, -20.0f, 0.0f,   135.0f, 20.0f};
    key_frames[5] = {10000, 0.0f,  0.0f,   0.0f,  0.0f};
}

MissionTestComplex::~MissionTestComplex() {
}

const KeyFrame* MissionTestComplex::getKeyFrames(int& num_frames) const {

    num_frames = NUM_KEY_FRAMES;
    return key_frames;
}
