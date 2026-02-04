#include "sbus_rescaler.hpp"
#include <algorithm>

namespace nokolat {

// デフォルト閾値の初期化
SBUSRescaler::Thresholds SBUSRescaler::default_thresholds = {};

// ===== 線形補間の実装 =====
float SBUSRescaler::linearMap(uint16_t value, uint16_t in_min, uint16_t in_max,
                               float out_min, float out_max) {

    // 入力値を範囲内にクランプ
    if (value <= in_min) {

        return out_min;
    }

    if (value >= in_max) {

        return out_max;
    }

    // 線形補間
    float normalized = static_cast<float>(value - in_min) / static_cast<float>(in_max - in_min);
    return out_min + normalized * (out_max - out_min);
}

// ===== スロットル変換: SBUS値 -> 0~100 =====
float SBUSRescaler::rescaleThrottle(uint16_t sbus_value, const Thresholds& thresholds) {

    return linearMap(sbus_value, thresholds.throttle_min, thresholds.throttle_max, 0.0f, 100.0f);
}

// ===== 制御入力変換: SBUS値 -> -100~100 =====
float SBUSRescaler::rescaleControl(uint16_t sbus_value, const Thresholds& thresholds) {

    if (sbus_value < thresholds.control_center) {

        // センター以下: min -> center を -100 -> 0 にマップ
        return linearMap(sbus_value, thresholds.control_min, thresholds.control_center, -100.0f, 0.0f);
    }
    else {

        // センター以上: center -> max を 0 -> 100 にマップ
        return linearMap(sbus_value, thresholds.control_center, thresholds.control_max, 0.0f, 100.0f);
    }
}

// ===== 3段階スイッチ変換: SBUS値 -> 0/1/2 =====
SwitchPosition SBUSRescaler::rescaleSwitch(uint16_t sbus_value, const Thresholds& thresholds) {

    if (sbus_value < thresholds.switch_low_threshold) {

        return SwitchPosition::LOW;
    }
    else if (sbus_value < thresholds.switch_high_threshold) {

        return SwitchPosition::MID;
    }
    else {

        return SwitchPosition::HIGH;
    }
}

// ===== 3段階スイッチ変換: SBUS値 -> 0/1/2 (uint8_t版) =====
uint8_t SBUSRescaler::rescaleSwitchInt(uint16_t sbus_value, const Thresholds& thresholds) {

    return static_cast<uint8_t>(rescaleSwitch(sbus_value, thresholds));
}

// ===== 配列からスロットル取得 =====
float SBUSRescaler::getThrottle(const std::array<uint16_t, 18>& sbus_data,
                                 SBUSChannel channel,
                                 const Thresholds& thresholds) {

    uint8_t index = static_cast<uint8_t>(channel);

    if (index >= sbus_data.size()) {

        return 0.0f;  // 範囲外の場合は0を返す
    }

    return rescaleThrottle(sbus_data[index], thresholds);
}

// ===== 配列から制御入力取得 =====
float SBUSRescaler::getControl(const std::array<uint16_t, 18>& sbus_data,
                                SBUSChannel channel,
                                const Thresholds& thresholds) {

    uint8_t index = static_cast<uint8_t>(channel);

    if (index >= sbus_data.size()) {

        return 0.0f;  // 範囲外の場合は0を返す
    }

    return rescaleControl(sbus_data[index], thresholds);
}

// ===== 配列からスイッチ取得 =====
SwitchPosition SBUSRescaler::getSwitch(const std::array<uint16_t, 18>& sbus_data,
                                        SBUSChannel channel,
                                        const Thresholds& thresholds) {

    uint8_t index = static_cast<uint8_t>(channel);

    if (index >= sbus_data.size()) {

        return SwitchPosition::LOW;  // 範囲外の場合はLOWを返す
    }

    return rescaleSwitch(sbus_data[index], thresholds);
}

// ===== 配列からスイッチ取得(uint8_t版) =====
uint8_t SBUSRescaler::getSwitchInt(const std::array<uint16_t, 18>& sbus_data,
                                    SBUSChannel channel,
                                    const Thresholds& thresholds) {

    return static_cast<uint8_t>(getSwitch(sbus_data, channel, thresholds));
}

// ===== デッドバンド適用 =====
float SBUSRescaler::applyDeadband(float value, float deadband) {

    if (value > -deadband && value < deadband) {

        return 0.0f;
    }

    return value;
}

// ===== 値をクランプ =====
float SBUSRescaler::clamp(float value, float min_val, float max_val) {

    return std::min(std::max(value, min_val), max_val);
}

// ===== 一括リスケーリング: SBUS生データ -> RescaledSBUSData =====
RescaledSBUSData SBUSRescaler::rescale(const std::array<uint16_t, 18>& sbus_data,
                                        const Thresholds& thresholds) {

    RescaledSBUSData result = {};

    // アナログチャネルのリスケーリング
    result.throttle = getThrottle(sbus_data, SBUSChannel::THROTTLE, thresholds);
    result.aileron = getControl(sbus_data, SBUSChannel::ROLL, thresholds);
    result.elevator = getControl(sbus_data, SBUSChannel::PITCH, thresholds);
    result.rudder = getControl(sbus_data, SBUSChannel::YAW, thresholds);

    // AUXチャネルのリスケーリング（3段階スイッチを 0/1 に変換）
    // LOW -> 0, MID/HIGH -> 1
    uint8_t drop_switch = getSwitchInt(sbus_data, SBUSChannel::DROP, thresholds);
    result.aux6 = (drop_switch >= 1) ? 1 : 0;  // DROP を aux6 に割り当て

    uint8_t aux1_switch = getSwitchInt(sbus_data, SBUSChannel::AUX1, thresholds);
    result.aux1 = (aux1_switch >= 1) ? 1 : 0;

    uint8_t aux2_switch = getSwitchInt(sbus_data, SBUSChannel::AUX2, thresholds);
    result.aux2 = (aux2_switch >= 1) ? 1 : 0;

    uint8_t aux3_switch = getSwitchInt(sbus_data, SBUSChannel::AUX3, thresholds);
    result.aux3 = (aux3_switch >= 1) ? 1 : 0;

    uint8_t aux4_switch = getSwitchInt(sbus_data, SBUSChannel::AUX4, thresholds);
    result.aux4 = (aux4_switch >= 1) ? 1 : 0;

    uint8_t aux5_switch = getSwitchInt(sbus_data, SBUSChannel::AUX5, thresholds);
    result.aux5 = (aux5_switch >= 1) ? 1 : 0;

    return result;
}

} // namespace nokolat
