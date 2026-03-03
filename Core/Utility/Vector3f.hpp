#ifndef VECTOR3F_HPP
#define VECTOR3F_HPP

#include <cstdint>
#include <array>

// 3軸データのインデックス定義
enum class Axis : uint8_t {

    X = 0,
    Y = 1,
    Z = 2
};

// 3軸データ格納用構造体（float型）
struct Vector3f {

    std::array<float, 3> data = {0.0f, 0.0f, 0.0f};

    // Axis enum で直接アクセス（内部でインデックスに変換）
    float& operator[](Axis axis) {

        return data[static_cast<uint8_t>(axis)];
    }

    const float& operator[](Axis axis) const {

        return data[static_cast<uint8_t>(axis)];
    }

    // 名前付きアクセサ（参照）
    float& x() { return data[static_cast<uint8_t>(Axis::X)]; }
    float& y() { return data[static_cast<uint8_t>(Axis::Y)]; }
    float& z() { return data[static_cast<uint8_t>(Axis::Z)]; }

    // 名前付きアクセサ（const）
    float x() const { return data[static_cast<uint8_t>(Axis::X)]; }
    float y() const { return data[static_cast<uint8_t>(Axis::Y)]; }
    float z() const { return data[static_cast<uint8_t>(Axis::Z)]; }

    // 配列の先頭ポインタを取得
    float* getptr() { return data.data(); }
    const float* getptr() const { return data.data(); }
};

#endif // VECTOR3F_HPP
