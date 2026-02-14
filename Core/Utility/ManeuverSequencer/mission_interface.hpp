#ifndef MISSION_INTERFACE_HPP
#define MISSION_INTERFACE_HPP

#include <cstdint>
#include "maneuver_setpoint.hpp"

// ===============================================================
// ミッションの抽象基底クラス
// 各ミッション（自動レベルフライト、着陸など）はこれを継承する
// ===============================================================

class MissionInterface {
public:
    virtual ~MissionInterface() = default;

    // ===============================================================
    // 純粋仮想関数（各ミッションで実装が必須）
    // ===============================================================

    // 経過時間に基づいて目標セットポイントを計算
    // @param elapsed_time_ms: ミッション開始からの経過時間 [ms]
    // @return 現在の目標値
    virtual ManeuverSetpoint getSetpoint(uint32_t elapsed_time_ms) = 0;

    // ミッションが完了したかどうかを判定
    // @param elapsed_time_ms: ミッション開始からの経過時間 [ms]
    // @return true: 完了, false: 進行中
    virtual bool isCompleted(uint32_t elapsed_time_ms) = 0;

    // ミッション全体の実行時間を取得
    // @return ミッション実行時間 [ms]
    virtual uint32_t getDurationMs() const = 0;

    // ===============================================================
    // オプション：進捗情報提供用（デフォルト実装あり）
    // ===============================================================

    // ミッションの進捗度合い [0.0 - 1.0] を返す
    // @param elapsed_time_ms: ミッション開始からの経過時間 [ms]
    // @return 進捗率 (0.0 = 開始, 1.0 = 完了)
    virtual float getProgress(uint32_t elapsed_time_ms) const {
        if (getDurationMs() == 0) return 0.0f;
        float progress = static_cast<float>(elapsed_time_ms) / 
                        static_cast<float>(getDurationMs());
        return progress > 1.0f ? 1.0f : progress;
    }

    // ミッション名を取得（ログ出力用）
    virtual const char* getMissionName() const = 0;
};

#endif // MISSION_INTERFACE_HPP
