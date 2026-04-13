#ifndef SBUS_CONFIG_HPP
#define SBUS_CONFIG_HPP

#include <cstdint>

namespace SbusConfig {

    // ===== SBUS デフォルト値（手動操縦用） =====
    constexpr uint16_t SBUS_MIN = 352;
    constexpr uint16_t SBUS_MID = 1024;
    constexpr uint16_t SBUS_MAX = 1696;

    // ===== SBUS 補正値（自動操縦用） =====

    // スロットル
    constexpr uint16_t THROTTLE_MIN = 352;
    constexpr uint16_t THROTTLE_MAX = 1696;

    // エルロン（左）
    constexpr uint16_t AILERON_MIN = 360;
    constexpr uint16_t AILERON_MID = 1024;
    constexpr uint16_t AILERON_MAX = 1680;

    // エレベーター
    constexpr uint16_t ELEVATOR_MIN = 360;
    constexpr uint16_t ELEVATOR_MID = 1024;
    constexpr uint16_t ELEVATOR_MAX = 1680;

    // ラダー
    constexpr uint16_t RUDDER_MIN = 360;
    constexpr uint16_t RUDDER_MID = 1024;
    constexpr uint16_t RUDDER_MAX = 1680;

    // エルロン（右）
    constexpr uint16_t RIGHT_AILERON_MIN = 360;
    constexpr uint16_t RIGHT_AILERON_MID = 1024;
    constexpr uint16_t RIGHT_AILERON_MAX = 1680;

    // ===== 3段階スイッチ判定閾値 =====
    constexpr uint16_t SWITCH_LOW_THRESHOLD  = 750;
    constexpr uint16_t SWITCH_HIGH_THRESHOLD = 1500;

    // ===== SBUSチャンネルマッピング =====
    // プロポーショナル送信機のチャンネル番号 (0始まりインデックス)
    constexpr uint8_t CH_AILERON         = 0;   // ch1: エルロン（左）
    constexpr uint8_t CH_RUDDER          = 3;   // ch2: ラダー
    constexpr uint8_t CH_THROTTLE        = 2;   // ch3: スロットル
    constexpr uint8_t CH_ELEVATOR        = 1;   // ch4: エレベーター
    constexpr uint8_t CH_DROP            = 4;   // ch5: 投下装置
    constexpr uint8_t CH_RIGHT_AILERON   = 5;   // ch6: エルロン（右）
    constexpr uint8_t CH_AUTOFLY         = 6;   // ch7: 自動操縦フラグ
    constexpr uint8_t CH_SELECT_MISSION  = 7;   // ch8: ミッション選択
    constexpr uint8_t CH_AUTO_MISSION    = 8;   // ch9: 自動離着陸
    constexpr uint8_t CH_SAFETY          = 9;   // ch10: 安全装置
    constexpr uint8_t CH_PREFLIGHT_DEBUG = 10;  // ch11: プリフライトデバッグ
    constexpr uint8_t CH_FLIGHT_DEBUG    = 11;  // ch12: フライトデバッグ

} // namespace SbusConfig

#endif // SBUS_CONFIG_HPP
