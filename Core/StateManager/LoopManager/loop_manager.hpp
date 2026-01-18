#ifndef LOOP_MANAGER_HPP
#define LOOP_MANAGER_HPP

#include <Arduino.h>


class LoopManager {

    public:

        // コンストラクタ
        LoopManager(unsigned long loop_interval_us = 100000) {

            this->loop_interval_us = loop_interval_us;
            this->last_time_us = micros();
        }

        // 次ループまで待機が必要かどうかを判定
        bool isWaitNextLoop() {

            unsigned long current_time_us = micros();

            // 最初の実行時は待機しない
            if (last_time_us == 0) {

                last_time_us = current_time_us;
                return false;
            }

            // 指定時間経過しているかの判定
            unsigned long delta_time_us = current_time_us - last_time_us;

            if (delta_time_us >= loop_interval_us) {

                last_time_us = current_time_us;
                return false;
            }

            // 指定時間経過していない場合は待機
            return true;
        }

    private:

        // ループ間隔（マイクロ秒）
        unsigned long loop_interval_us = 1000;

        // 最後にループを実行した時刻
        unsigned long last_time_us = 0;
};

#endif // LOOP_MANAGER_HPP
