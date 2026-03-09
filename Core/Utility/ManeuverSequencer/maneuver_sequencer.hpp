#ifndef MANEUVER_SEQUENCER_HPP
#define MANEUVER_SEQUENCER_HPP

#include <cstdint>

// 前方宣言（完全な定義は maneuver_sequencer.cpp で include）
class MissionBase;

// チャンネルフラグ（ミッションが制御するチャンネルを示す）
namespace ChannelFlags {

    constexpr uint8_t ROLL     = 0x01;
    constexpr uint8_t PITCH    = 0x02;
    constexpr uint8_t YAW      = 0x04;
    constexpr uint8_t ALTITUDE = 0x08;

    constexpr uint8_t ATTITUDE = 0x07;  // ROLL | PITCH | YAW
    constexpr uint8_t ALL      = 0x0F;  // ROLL | PITCH | YAW | ALTITUDE
}

// キーフレーム構造体
// ミッション中の各フレームの時刻と目標値を保持する
struct KeyFrame {

    uint32_t time_ms;       // フレームの時刻 [ms]

    float    roll_deg;      // ロール目標値 [deg]
    float    pitch_deg;     // ピッチ目標値 [deg]
    float    yaw_deg;       // ヨー目標値 [deg]

    float    altitude_m;    // 高度目標値 [m]
};

// マネューバーシーケンサークラス
// 現在時刻に対してキーフレームの線形補間から目標値を計算する
class ManeuverSequencer {

    public:

        ManeuverSequencer();
        ~ManeuverSequencer();

        // ミッション開始（現在時刻を記録）
        // [in] mission - ミッションのポインタ
        void startMission(const MissionBase* mission);

        // ミッション完了判定
        // [return] ミッションが完了している場合 true、未完了の場合 false
        bool isMissionComplete() const;

        // 現在時刻での目標値を計算する
        // [out] roll_deg - ロール目標値 [deg]
        // [out] pitch_deg - ピッチ目標値 [deg]
        // [out] yaw_deg - ヨー目標値 [deg]
        // [out] altitude_m - 高度目標値 [m]
        // [return] 取得成功時 true、失敗時 false（ミッション未設定やキーフレーム不足）
        bool getTargetValues(
            float& roll_deg,
            float& pitch_deg,
            float& yaw_deg,
            float& altitude_m
        ) const;

        // アクティブチャンネルのビットマスクを取得する
        // [return] ChannelFlags の組み合わせ（ミッション未設定の場合 0）
        uint8_t getActiveChannels() const;

    private:

        // 与えられた時刻に対して、その前後のキーフレームを検索する
        // [in] current_ms - 現在時刻 [ms]
        // [in] key_frames - キーフレーム配列
        // [in] num_frames - キーフレーム数
        // [out] frame_index - 現在時刻の直前のキーフレームのインデックス
        // [return] 有効なフレームペアが見つかった場合 true、それ以外 false
        bool findFrameIndices(
            uint32_t current_ms,
            const KeyFrame* key_frames,
            int num_frames,
            int& frame_index
        ) const;

        // 線形補間により、与えられた値を計算する
        // [in] time_a - 補間開始時刻 [ms]
        // [in] value_a - 補間開始値
        // [in] time_b - 補間終了時刻 [ms]
        // [in] value_b - 補間終了値
        // [in] current_ms - 現在時刻 [ms]
        // [return] 補間後の値
        float linearInterpolation(
            uint32_t time_a,
            float value_a,
            uint32_t time_b,
            float value_b,
            uint32_t current_ms
        ) const;

        const MissionBase* current_mission_   = nullptr;
        uint32_t           mission_start_ms_  = 0;
        bool               mission_started_   = false;
};

#endif // MANEUVER_SEQUENCER_HPP
