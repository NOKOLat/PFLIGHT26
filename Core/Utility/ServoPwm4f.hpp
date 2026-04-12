#ifndef SERVO_PWM_4F_HPP
#define SERVO_PWM_4F_HPP

#include <cstdint>
#include <array>

// サーボチャンネルのインデックス定義
// servo_pwm 配列の論理的な順序
enum class ServoIndex : uint8_t {
    ELEVATOR      = 0,  // エレベーター
    RUDDER        = 1,  // ラダー
    RIGHT_AILERON = 2,  // 右エルロン
    LEFT_AILERON  = 3,  // 左エルロン
    DROP          = 4,  // 投下装置
};

// サーボ角度格納用構造体（float型）
// 各要素は [-90 ~ +90] deg
struct ServoPwm4f {

    std::array<float, 5> data = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

    // ServoIndex enum でアクセス（内部でインデックスに変換）
    float& operator[](ServoIndex idx) {
        return data[static_cast<uint8_t>(idx)];
    }
    const float& operator[](ServoIndex idx) const {
        return data[static_cast<uint8_t>(idx)];
    }

    // 名前付きアクセサ（参照）
    float& elevator()      { return data[static_cast<uint8_t>(ServoIndex::ELEVATOR)]; }
    float& rudder()        { return data[static_cast<uint8_t>(ServoIndex::RUDDER)]; }
    float& right_aileron() { return data[static_cast<uint8_t>(ServoIndex::RIGHT_AILERON)]; }
    float& left_aileron()  { return data[static_cast<uint8_t>(ServoIndex::LEFT_AILERON)]; }
    float& drop()          { return data[static_cast<uint8_t>(ServoIndex::DROP)]; }

    // 名前付きアクセサ（const）
    float elevator()      const { return data[static_cast<uint8_t>(ServoIndex::ELEVATOR)]; }
    float rudder()        const { return data[static_cast<uint8_t>(ServoIndex::RUDDER)]; }
    float right_aileron() const { return data[static_cast<uint8_t>(ServoIndex::RIGHT_AILERON)]; }
    float left_aileron()  const { return data[static_cast<uint8_t>(ServoIndex::LEFT_AILERON)]; }
    float drop()          const { return data[static_cast<uint8_t>(ServoIndex::DROP)]; }

    // 配列の先頭ポインタを取得
    float* getptr() { return data.data(); }
    const float* getptr() const { return data.data(); }
};

#endif // SERVO_PWM_4F_HPP
