#ifndef MISSIONS_HPP
#define MISSIONS_HPP

#include "maneuver_sequencer.hpp"

// ==================================================
// テスト用シンプルミッション
// 0msから3000msまで、段階的に目標値を変化させるミッション
// ==================================================
class MissionTestSimple : public MissionBase {

    public:

        MissionTestSimple();
        ~MissionTestSimple();

        const KeyFrame* getKeyFrames(int& num_frames) const override;

    private:

        static constexpr int NUM_KEY_FRAMES = 4;
        KeyFrame key_frames[NUM_KEY_FRAMES];
};

// ==================================================
// テスト用複雑ミッション
// 6つのキーフレームを使用した複雑な飛行経路
// 離陸 -> 上昇 -> 旋回 -> 高度変更 -> 旋回 -> 着陸
// ==================================================
class MissionTestComplex : public MissionBase {

    public:

        MissionTestComplex();
        ~MissionTestComplex();

        const KeyFrame* getKeyFrames(int& num_frames) const override;

    private:

        static constexpr int NUM_KEY_FRAMES = 6;
        KeyFrame key_frames[NUM_KEY_FRAMES];
};

#endif // MISSIONS_HPP
