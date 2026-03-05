#ifndef MOVING_AVERAGE_HPP
#define MOVING_AVERAGE_HPP

#include <cstdint>
#include <array>

// 汎用の移動平均を計算するユーティリティクラス
// リングバッファ方式で指定されたサンプル数の移動平均を保持する
template<typename T, uint8_t WINDOW>
class MovingAverage {

public:

    // 新しい値を追加し、移動平均を返す
    // バッファが満杯になる前は有効サンプル数のみで平均を計算する
    T update(T value) {

        buffer_[idx_] = value;
        idx_ = (idx_ + 1) % WINDOW;

        if (count_ < WINDOW) {
            count_++;
        }

        T sum = static_cast<T>(0);
        for (uint8_t i = 0; i < count_; i++) {
            sum += buffer_[i];
        }
        return sum / static_cast<T>(count_);
    }

    // バッファをリセットする
    void reset() {
        buffer_ = {};
        idx_    = 0;
        count_  = 0;
    }

private:

    std::array<T, WINDOW> buffer_ = {};
    uint8_t idx_   = 0;
    uint8_t count_ = 0;
};

#endif // MOVING_AVERAGE_HPP
