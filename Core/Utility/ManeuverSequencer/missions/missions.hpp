#ifndef MISSIONS_HPP
#define MISSIONS_HPP

#include <cstdint>
#include "../maneuver_sequencer.hpp"

// ================================================================================
// MissionBase - ミッション基底クラス
// ================================================================================
// 各ミッション固有のキーフレームデータを提供するインターフェース
class MissionBase {

    public:

        virtual ~MissionBase() = default;

        // キーフレーム配列を取得する
        // [out] num_frames - キーフレーム数
        // [return] KeyFrame 配列へのポインタ
        virtual const KeyFrame* getKeyFrames(int& num_frames) const = 0;

        // このミッションが制御するチャンネルのビットマスクを取得する
        // [return] ChannelFlags の組み合わせ（例: ChannelFlags::ATTITUDE）
        virtual uint8_t getActiveChannels() const = 0;
};

// ================================================================================
// MissionTest - テスト用ミッション
// ================================================================================
// 4つのキーフレームを持つテスト用ミッション
// 全チャンネルを制御
class MissionTest : public MissionBase {

    public:

        MissionTest();
        ~MissionTest() = default;

        // キーフレーム配列を取得する
        const KeyFrame* getKeyFrames(int& num_frames) const override;

        // このミッションが制御するチャンネルを取得する
        uint8_t getActiveChannels() const override;

    private:

        static constexpr int NUM_KEY_FRAMES = 4;
        KeyFrame key_frames[NUM_KEY_FRAMES];
};

// ================================================================================
// MissionLevelFlight - 水平飛行ミッション
// ================================================================================
// t=0～15秒、すべての角度目標値が0度
// 高度制御は使用しない（ATTITUDE チャンネルのみ制御）
class MissionLevelFlight : public MissionBase {

    public:

        MissionLevelFlight();
        ~MissionLevelFlight() = default;

        // キーフレーム配列を取得する
        const KeyFrame* getKeyFrames(int& num_frames) const override;

        // このミッションが制御するチャンネルを取得する
        // 高度制御を使用しないため ATTITUDE のみ
        uint8_t getActiveChannels() const override;

    private:

        static constexpr int NUM_KEY_FRAMES = 2;
        KeyFrame key_frames[NUM_KEY_FRAMES];
};

// ================================================================================
// MissionLevelTurn - レベルターンミッション
// ================================================================================
// t=0～10秒、バンク角を増やしながら旋回する
// 高度制御は使用しない（ATTITUDE チャンネルのみ制御）
class MissionLevelTurn : public MissionBase {

    public:

        MissionLevelTurn();
        ~MissionLevelTurn() = default;

        // キーフレーム配列を取得する
        const KeyFrame* getKeyFrames(int& num_frames) const override;

        // このミッションが制御するチャンネルを取得する
        // 高度制御を使用しないため ATTITUDE のみ
        uint8_t getActiveChannels() const override;

    private:

        static constexpr int NUM_KEY_FRAMES = 2;
        KeyFrame key_frames[NUM_KEY_FRAMES];
};

#endif // MISSIONS_HPP
