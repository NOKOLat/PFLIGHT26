#ifndef MOTOR_PWM_2F_HPP
#define MOTOR_PWM_2F_HPP

#include <cstdint>
#include <array>

// モーターチャンネルのインデックス定義
// motor_pwm 配列の論理的な順序
enum class MotorIndex : uint8_t {
    RIGHT = 0,  // 右モーター
    LEFT  = 1,  // 左モーター
};

// モーター速度格納用構造体（float型）
// 各要素は [0 ~ 100] %
struct MotorPwm2f {

    std::array<float, 2> data = {0.0f, 0.0f};

    // MotorIndex enum でアクセス（内部でインデックスに変換）
    float& operator[](MotorIndex idx) {
        return data[static_cast<uint8_t>(idx)];
    }
    const float& operator[](MotorIndex idx) const {
        return data[static_cast<uint8_t>(idx)];
    }

    // 名前付きアクセサ（参照）
    float& right() { return data[static_cast<uint8_t>(MotorIndex::RIGHT)]; }
    float& left()  { return data[static_cast<uint8_t>(MotorIndex::LEFT)]; }

    // 名前付きアクセサ（const）
    float right() const { return data[static_cast<uint8_t>(MotorIndex::RIGHT)]; }
    float left()  const { return data[static_cast<uint8_t>(MotorIndex::LEFT)]; }

    // 配列の先頭ポインタを取得
    float* getptr() { return data.data(); }
    const float* getptr() const { return data.data(); }
};

#endif // MOTOR_PWM_2F_HPP
