#ifndef SBUS_RESCALER_HPP
#define SBUS_RESCALER_HPP

#include <cstdint>
#include <array>
#include "../Config/sbus_config.hpp"

namespace nokolat {

// ===== リスケーリング後のSBUSデータ構造体 =====
// SBUSの生データを制御用の値に変換した構造体
struct RescaledSBUSData {

    // アナログチャネル
    float throttle;      // スロットル [0~100] %
    float aileron;       // エルロン (ロール) [-100~100] %
    float elevator;      // エレベーター (ピッチ) [-100~100] %
    float rudder;        // ラダー (ヨー) [-100~100] %
    float right_aileron;   // 右エルロン [-90~90] deg

    // AUXチャネル
    uint8_t autofly;         // 自動操縦フラグ [0 / 1 / 2]
    uint8_t selectmission;   // ミッション選択 [0 / 1 / 2]
    uint8_t auto_mission;    // 自動離着陸用 [0 / 1 / 2]
    uint8_t safety;          // 安全装置 [0:解除 / 1:有効]
    uint8_t drop;            // 投下装置トリガー [0 / 1 / 2]
    uint8_t preflight_debug; // プリフライトデバッグ [0 / 1 / 2]
    uint8_t flight_debug;    // フライトデバッグ [0 / 1 / 2]
};

// ===== SBUSチャンネルのインデックス定義 =====
// プロポーショナル送信機のチャンネルマッピング
enum class SBUSChannel : uint8_t {

    THROTTLE        = SbusConfig::CH_THROTTLE,
    AILERON         = SbusConfig::CH_AILERON,
    RUDDER          = SbusConfig::CH_RUDDER,
    ELEVATOR        = SbusConfig::CH_ELEVATOR,
    DROP            = SbusConfig::CH_DROP,
    RIGHT_AILERON   = SbusConfig::CH_RIGHT_AILERON,
    AUTOFLY         = SbusConfig::CH_AUTOFLY,
    SELECT_MISSION  = SbusConfig::CH_SELECT_MISSION,
    AUTO_MISSION    = SbusConfig::CH_AUTO_MISSION,
    SAFETY          = SbusConfig::CH_SAFETY,
    PREFLIGHT_DEBUG = SbusConfig::CH_PREFLIGHT_DEBUG,
    FLIGHT_DEBUG    = SbusConfig::CH_FLIGHT_DEBUG
};

// ===== 3段階スイッチの状態定義 =====
enum class SwitchPosition : uint8_t {

    LOW = 0,
    MID = 1,
    HIGH = 2
};

// ===== SBUSリスケーラークラス =====
// SBUSの生データ（360~1692）を制御用の値にリスケールする静的メソッド群
class SBUSRescaler {
public:
    // ===== SBUS生データの範囲定数 =====
    static constexpr uint16_t SBUS_MIN = SbusConfig::SBUS_MIN;
    static constexpr uint16_t SBUS_MID = SbusConfig::SBUS_MID;
    static constexpr uint16_t SBUS_MAX = SbusConfig::SBUS_MAX;

    // ===== 閾値設定用構造体 =====
    struct Thresholds {

        // スロットル: 0~100の範囲にマッピング
        uint16_t throttle_min = SbusConfig::SBUS_MIN;
        uint16_t throttle_max = SbusConfig::SBUS_MAX;

        // ロール、ピッチ、ヨー: -100~100の範囲にマッピング
        uint16_t control_min = SbusConfig::SBUS_MIN;
        uint16_t control_center = SbusConfig::SBUS_MID;
        uint16_t control_max = SbusConfig::SBUS_MAX;

        // 3段階スイッチの閾値 (0~750: LOW / 751~1500: MID / 1501~2047: HIGH)
        uint16_t switch_low_threshold = SbusConfig::SWITCH_LOW_THRESHOLD;    // LOW/MID境界
        uint16_t switch_high_threshold = SbusConfig::SWITCH_HIGH_THRESHOLD;  // MID/HIGH境界
    };

    // デフォルト閾値
    static Thresholds default_thresholds;

    // ===== スロットル変換メソッド =====
    // SBUS値を0~100の範囲にリスケール
    static float rescaleThrottle(uint16_t sbus_value,
                                  const Thresholds& thresholds = default_thresholds);

    // ===== 制御入力変換メソッド =====
    // SBUS値を-100~100の範囲にリスケール（ロール、ピッチ、ヨー用）
    static float rescaleControl(uint16_t sbus_value,
                                 const Thresholds& thresholds = default_thresholds);

    // ===== 3段階スイッチ変換メソッド =====
    // SBUS値をSwitchPosition（LOW/MID/HIGH）に変換
    static SwitchPosition rescaleSwitch(uint16_t sbus_value,
                                         const Thresholds& thresholds = default_thresholds);

    // SBUS値を0/1/2に変換（uint8_t版）
    static uint8_t rescaleSwitchInt(uint16_t sbus_value,
                                     const Thresholds& thresholds = default_thresholds);

    // ===== 配列からチャンネルを指定して変換 =====
    // スロットル取得（0~100）
    static float getThrottle(const std::array<uint16_t, 18>& sbus_data,
                             SBUSChannel channel = SBUSChannel::THROTTLE,
                             const Thresholds& thresholds = default_thresholds);

    // 制御入力取得（-100~100）
    static float getControl(const std::array<uint16_t, 18>& sbus_data,
                            SBUSChannel channel,
                            const Thresholds& thresholds = default_thresholds);

    // 3段階スイッチ取得（SwitchPosition）
    static SwitchPosition getSwitch(const std::array<uint16_t, 18>& sbus_data,
                                     SBUSChannel channel = SBUSChannel::DROP,
                                     const Thresholds& thresholds = default_thresholds);

    // 3段階スイッチ取得（uint8_t版）
    static uint8_t getSwitchInt(const std::array<uint16_t, 18>& sbus_data,
                                 SBUSChannel channel = SBUSChannel::DROP,
                                 const Thresholds& thresholds = default_thresholds);

    // ===== 一括リスケーリングメソッド =====
    // SBUS生データから RescaledSBUSData を生成
    static RescaledSBUSData rescale(const std::array<uint16_t, 18>& sbus_data,
                                     const Thresholds& thresholds = default_thresholds);

    // ===== ユーティリティメソッド =====
    // デッドバンド適用（小さな値を0にする）
    static float applyDeadband(float value, float deadband = 5.0f);

    // 値をクランプ
    static float clamp(float value, float min_val, float max_val);

private:

    // 線形補間
    static float linearMap(uint16_t value, uint16_t in_min, uint16_t in_max,
                           float out_min, float out_max);
};

} // namespace nokolat

#endif // SBUS_RESCALER_HPP
