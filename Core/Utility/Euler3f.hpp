#ifndef EULER3F_HPP
#define EULER3F_HPP

#include <cstdint>
#include <array>

// オイラー角軸のインデックス定義
enum class EulerAxis : uint8_t {
    ROLL  = 0,
    PITCH = 1,
    YAW   = 2
};

// オイラー角データ格納用構造体（float型）
// roll=0, pitch=1, yaw=2 の順で格納
struct Euler3f {

    std::array<float, 3> data = {0.0f, 0.0f, 0.0f};

    // EulerAxis enum でアクセス（Vector3f の Axis と同じパターン）
    float& operator[](EulerAxis axis) {
        return data[static_cast<uint8_t>(axis)];
    }
    const float& operator[](EulerAxis axis) const {
        return data[static_cast<uint8_t>(axis)];
    }

    // 名前付きアクセサ（参照）
    float& roll()  { return data[static_cast<uint8_t>(EulerAxis::ROLL)]; }
    float& pitch() { return data[static_cast<uint8_t>(EulerAxis::PITCH)]; }
    float& yaw()   { return data[static_cast<uint8_t>(EulerAxis::YAW)]; }

    // 名前付きアクセサ（const）
    float roll()  const { return data[static_cast<uint8_t>(EulerAxis::ROLL)]; }
    float pitch() const { return data[static_cast<uint8_t>(EulerAxis::PITCH)]; }
    float yaw()   const { return data[static_cast<uint8_t>(EulerAxis::YAW)]; }

    // 配列の先頭ポインタを取得
    float* getptr() { return data.data(); }
    const float* getptr() const { return data.data(); }
};

#endif // EULER3F_HPP
