
#ifndef AUTOMATIC_LEVEL_FLIGHT_HPP
#define AUTOMATIC_LEVEL_FLIGHT_HPP

#include <cstdint>
#include <array>
#include "../maneuver_setpoint.hpp"

// ===============================================================
// 自動レベルフライミッション（キーフレーム定義）
//
// 概要:
//   航空機を水平に飛行させるミッション
//   デバッグや基本的な自動操縦テスト用
//
// 動作:
//   - 目標ロール角: 0°（常に水平）
//   - 目標ピッチ角: 0°（常に水平）
//   - スロットル: 一定値で維持
// ===============================================================

namespace missions {

class AutomaticLevelFlight {
public:
    // ミッション実行時間 [ms]
    static constexpr uint32_t DURATION_MS = 30000;  // 30秒

    // キーフレーム配列
    // (時刻, ロール, ピッチ, ヨー, スロットル)
    static constexpr std::array<ManeuverKeyframe, 2> KEYFRAMES = {{
        {0,     0.0f, 0.0f, 0.0f, 50.0f},  // t=0ms: 開始
        {10, 0.0f, 0.0f, 0.0f, 50.0f},  // t=30s: 終了
    }};
};

}  // namespace missions

#endif // AUTOMATIC_LEVEL_FLIGHT_HPP