#ifndef MANEUVER_SEQUENCER_HPP
#define MANEUVER_SEQUENCER_HPP

#include <cstdint>

// キーフレーム構造体
// ミッション中の各フレームの時刻と目標値を保持する
struct KeyFrame {
    uint32_t time_ms;       // フレームの時刻[ms]

    float    roll_deg;      // ロール目標値[deg]
    float    pitch_deg;     // ピッチ目標値[deg]
    float    yaw_deg;       // ヨー目標値[deg]

    float    altitude_m;    // 高度目標値[m]
};

// ミッション基底クラス
// 各ミッション固有のキーフレームデータを提供するインターフェース
class MissionBase {

    public:

        virtual ~MissionBase() = default;

        // キーフレーム配列を取得する
        // [out] numFrames - キーフレーム数
        // [return] KeyFrame配列へのポインタ
        virtual const KeyFrame* getKeyFrames(int& num_frames) const = 0;
};

// マネューバーシーケンサークラス
// 現在時刻に対してキーフレームの線形補間から目標値を計算する
class ManeuverSequencer {

    public:

        ManeuverSequencer();
        ~ManeuverSequencer();

        // ミッション完了判定
        // [in] current_time_ms - 現在時刻[ms]
        // [in] mission - ミッション基底クラスのポインタ
        // [return] ミッションが完了している場合true、未完了の場合false
        bool isMissionComplete(uint32_t current_time_ms, const MissionBase* mission) const;

        // 現在時刻での目標値を計算する（ロール角）
        // [in] current_time_ms - 現在時刻[ms]
        // [in] mission - ミッション基底クラスのポインタ
        // [return] ロール目標値[deg]
        float getTargetRoll(uint32_t current_time_ms, const MissionBase* mission) const;

        // 現在時刻での目標値を計算する（ピッチ角）
        // [in] current_time_ms - 現在時刻[ms]
        // [in] mission - ミッション基底クラスのポインタ
        // [return] ピッチ目標値[deg]
        float getTargetPitch(uint32_t current_time_ms, const MissionBase* mission) const;

        // 現在時刻での目標値を計算する（ヨー角）
        // [in] current_time_ms - 現在時刻[ms]
        // [in] mission - ミッション基底クラスのポインタ
        // [return] ヨー目標値[deg]
        float getTargetYaw(uint32_t current_time_ms, const MissionBase* mission) const;

        // 現在時刻での目標値を計算する（高度）
        // [in] current_time_ms - 現在時刻[ms]
        // [in] mission - ミッション基底クラスのポインタ
        // [return] 高度目標値[m]
        float getTargetAltitude(uint32_t current_time_ms, const MissionBase* mission) const;

        // 全目標値を一度に取得する
        // [in] current_time_ms - 現在時刻[ms]
        // [in] mission - ミッション基底クラスのポインタ
        // [out] target_roll_deg - ロール目標値[deg]
        // [out] target_pitch_deg - ピッチ目標値[deg]
        // [out] target_yaw_deg - ヨー目標値[deg]
        // [out] target_altitude_m - 高度目標値[m]
        // [return] 成功時true、失敗時false
        bool getTargetValues(
            uint32_t current_time_ms,
            const MissionBase* mission,
            float& target_roll_deg,
            float& target_pitch_deg,
            float& target_yaw_deg,
            float& target_altitude_m
        ) const;

    private:

        // 与えられた時刻に対して、その前後のキーフレームを検索する
        // [in] current_time_ms - 現在時刻[ms]
        // [in] key_frames - キーフレーム配列
        // [in] num_frames - キーフレーム数
        // [out] frame_index - 現在時刻の直前のキーフレームのインデックス
        // [return] 有効なフレームペアが見つかった場合true、それ以外false
        bool findFrameIndices(
            uint32_t current_time_ms,
            const KeyFrame* key_frames,
            int num_frames,
            int& frame_index
        ) const;

        // 線形補間により、与えられた値を計算する
        // [in] time_a - 補間開始時刻[ms]
        // [in] value_a - 補間開始値
        // [in] time_b - 補間終了時刻[ms]
        // [in] value_b - 補間終了値
        // [in] current_time_ms - 現在時刻[ms]
        // [return] 補間後の値
        float linearInterpolation(
            uint32_t time_a,
            float value_a,
            uint32_t time_b,
            float value_b,
            uint32_t current_time_ms
        ) const;
};

#endif // MANEUVER_SEQUENCER_HPP
