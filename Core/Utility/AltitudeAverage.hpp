#ifndef ALTITUDE_AVERAGE_HPP
#define ALTITUDE_AVERAGE_HPP

#include <cstdint>
#include <array>

// 直近高度の移動平均を計算するユーティリティクラス
// リングバッファ方式で WINDOW サンプルの移動平均を保持する
class AltitudeAverage {

public:

    // 新しい高度値を追加し、移動平均を返す
    // バッファが満杯になる前は有効サンプル数のみで平均を計算する
    float update(float altitude) {

        buffer_[idx_] = altitude;
        idx_ = (idx_ + 1) % WINDOW;

        if (count_ < WINDOW) {
            count_++;
        }

        float sum = 0.0f;
        for (uint8_t i = 0; i < count_; i++) {
            sum += buffer_[i];
        }
        return sum / static_cast<float>(count_);
    }

    // バッファをリセットする
    void reset() {
        buffer_ = {};
        idx_    = 0;
        count_  = 0;
    }

private:

    static constexpr uint8_t WINDOW = 20;
    std::array<float, WINDOW> buffer_ = {};
    uint8_t idx_   = 0;
    uint8_t count_ = 0;
};

#endif // ALTITUDE_AVERAGE_HPP
