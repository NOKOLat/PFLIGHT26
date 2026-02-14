#ifndef MANEUVER_SEQUENCER_HPP
#define MANEUVER_SEQUENCER_HPP

#include <cstdint>
#include <array>
#include "maneuver_setpoint.hpp"

// ===============================================================
// 自動操縦用ミッションシーケンサー
//
// 概要:
//   キーフレーム配列から現在の目標セットポイントを計算して返す
//   キーフレーム間は線形補間される
//
// 使用例:
//   std::array<ManeuverKeyframe, 2> keyframes = {{
//       {0, 0.0f, 0.0f, 0.0f, 50.0f},      // t=0ms 開始
//       {30000, 0.0f, 0.0f, 0.0f, 50.0f}   // t=30s 終了
//   }};
//   
//   ManeuverSequencer sequencer;
//   sequencer.initialize(keyframes.data(), keyframes.size(), 30000);
//   
//   while (running) {
//       sequencer.update(elapsed_time_ms);
//       auto target = sequencer.getSetpoint();
//   }
// ===============================================================

class ManeuverSequencer {
public:
    ManeuverSequencer() = default;
    ~ManeuverSequencer() = default;

    // ===============================================================
    // ライフサイクルメソッド
    // ===============================================================

    // キーフレーム配列でミッションを初期化
    // @param keyframes: キーフレーム配列のポインタ
    // @param keyframe_count: キーフレーム数
    // @param duration_ms: ミッション全体の実行時間 [ms]
    // @return 初期化成功: true, 失敗: false
    bool initialize(const ManeuverKeyframe* keyframes, 
                   uint32_t keyframe_count, 
                   uint32_t duration_ms);

    // 現在時刻でミッションを更新
    // @param current_time_ms: 現在時刻 [ms]
    void update(uint32_t current_time_ms);

    // ===============================================================
    // アクセッサメソッド
    // ===============================================================

    // 現在のミッションセットポイントを取得
    // @return 線形補間された目標セットポイント
    ManeuverSetpoint getSetpoint() const {
        return current_setpoint_;
    }

    // ミッション進行状況を取得
    // @return 進捗率 [0.0 - 1.0]
    float getProgress() const {
        if (duration_ms_ == 0) return 0.0f;
        float progress = static_cast<float>(elapsed_time_ms_) / 
                        static_cast<float>(duration_ms_);
        return progress > 1.0f ? 1.0f : progress;
    }

    // ミッション完了判定
    // @return true: 完了, false: 進行中
    bool isCompleted() const {
        return elapsed_time_ms_ >= duration_ms_;
    }

    // ミッション実行中判定
    // @return true: 実行中, false: 未初期化または完了
    bool isRunning() const {
        return keyframes_ != nullptr && !isCompleted();
    }

    // 経過時間を取得 [ms]
    uint32_t getElapsedTime() const {
        return elapsed_time_ms_;
    }

    // ===============================================================
    // ユーティリティメソッド
    // ===============================================================

    // ミッションをリセット（再スタート）
    void reset();

private:
    // ===============================================================
    // プライベートメンバ変数
    // ===============================================================

    // キーフレーム配列へのポインタ
    const ManeuverKeyframe* keyframes_ = nullptr;

    // キーフレーム数
    uint32_t keyframe_count_ = 0;

    // ミッション全体の実行時間 [ms]
    uint32_t duration_ms_ = 0;

    // ミッション開始からの経過時間 [ms]
    uint32_t elapsed_time_ms_ = 0;

    // キャッシュされた現在のセットポイント
    ManeuverSetpoint current_setpoint_;

    // ===============================================================
    // プライベートメソッド
    // ===============================================================

    // 経過時間に基づいてセットポイントを計算（線形補間）
    void computeSetpoint();

    // 指定時刻に対応するキーフレームペアを探す
    // @param elapsed_time_ms: 経過時間 [ms]
    // @return キーフレームのインデックス（frame_aのインデックス）
    uint32_t findKeyframeIndex(uint32_t elapsed_time_ms) const;
};

#endif // MANEUVER_SEQUENCER_HPP
