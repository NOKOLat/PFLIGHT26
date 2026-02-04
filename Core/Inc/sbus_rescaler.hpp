#ifndef SBUS_RESCALER_HPP
#define SBUS_RESCALER_HPP

#include <cstdint>
#include <array>

namespace nokolat {

// ===== リスケーリング後のSBUSデータ構造体 =====
// SBUSの生データを制御用の値に変換した構造体
struct RescaledSBUSData {

    // アナログチャネル
    float throttle;      // スロットル [0~100] %
    float aileron;       // エルロン (ロール) [-100~100] %
    float elevator;      // エレベーター (ピッチ) [-100~100] %
    float rudder;        // ラダー (ヨー) [-100~100] %

    // AUXチャネル
    uint8_t aux1;        // AUX1 [0 or 1]
    uint8_t aux2;        // AUX2 [0 or 1]
    uint8_t aux3;        // AUX3 [0 or 1]
    uint8_t aux4;        // AUX4 [0 or 1]
    uint8_t aux5;        // AUX5 [0 or 1]
    uint8_t aux6;        // 投下装置（DROP） [0 or 1]
};

// ===== SBUSチャンネルのインデックス定義 =====
// プロポーショナル送信機のチャンネルマッピング
enum class SBUSChannel : uint8_t {

    THROTTLE = 0,    // スロットル
    ROLL = 1,        // ロール（エルロン）
    PITCH = 2,       // ピッチ（エレベーター）
    YAW = 3,         // ヨー（ラダー）
    DROP = 4,        // 投下装置
    AUX1 = 5,        // 補助チャンネル1
    AUX2 = 6,        // 補助チャンネル2
    AUX3 = 7,        // 補助チャンネル3
    AUX4 = 8,        // 補助チャンネル4
    AUX5 = 9         // 補助チャンネル5
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
    static constexpr uint16_t SBUS_MIN = 360;
    static constexpr uint16_t SBUS_MID = 1024;
    static constexpr uint16_t SBUS_MAX = 1692;

    // ===== 閾値設定用構造体 =====
    struct Thresholds {

        // スロットル: 0~100の範囲にマッピング
        uint16_t throttle_min = SBUS_MIN;
        uint16_t throttle_max = SBUS_MAX;

        // ロール、ピッチ、ヨー: -100~100の範囲にマッピング
        uint16_t control_min = SBUS_MIN;
        uint16_t control_center = SBUS_MID;
        uint16_t control_max = SBUS_MAX;

        // 投下装置など3段階スイッチの閾値
        uint16_t switch_low_threshold = 600;    // LOW/MID境界
        uint16_t switch_high_threshold = 1400;  // MID/HIGH境界
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
