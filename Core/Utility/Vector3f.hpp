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

    // 配列の先頭ポインタを取得
    float* getptr() {
        return data.data();
    }

    // セッターメソッド
    void setX(float x) { data[static_cast<uint8_t>(Axis::X)] = x; }
    void setY(float y) { data[static_cast<uint8_t>(Axis::Y)] = y; }
    void setZ(float z) { data[static_cast<uint8_t>(Axis::Z)] = z; }

    // ゲッターメソッド
    float getX() const { return data[static_cast<uint8_t>(Axis::X)]; }
    float getY() const { return data[static_cast<uint8_t>(Axis::Y)]; }
    float getZ() const { return data[static_cast<uint8_t>(Axis::Z)]; }
};

#endif // VECTOR3F_HPP
