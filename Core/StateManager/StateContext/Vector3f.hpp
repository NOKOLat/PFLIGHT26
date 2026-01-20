#ifndef VECTOR3F_HPP
#define VECTOR3F_HPP

#include <cstdint>

// 3軸データのインデックス定義
enum class Axis : uint8_t {

    X = 0,
    Y = 1,
    Z = 2
};

// 3軸データ格納用構造体（float型）
struct Vector3f {

    float data[3] = {0.0f, 0.0f, 0.0f};

    // Axis enum で直接アクセス（内部でインデックスに変換）
    float& operator[](Axis axis) {

        return data[static_cast<uint8_t>(axis)];
    }

    const float& operator[](Axis axis) const {

        return data[static_cast<uint8_t>(axis)];
    }
};

#endif // VECTOR3F_HPP