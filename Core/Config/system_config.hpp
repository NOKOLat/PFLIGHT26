#ifndef SYSTEM_CONFIG_HPP
#define SYSTEM_CONFIG_HPP

#include <cstdint>

namespace SystemConfig {

    // ===== メインループ周期設定 =====
    // 制御ループの実行周期（マイクロ秒）
    // 20000μs = 20ms = 50Hz が標準
    constexpr uint32_t MAIN_LOOP_PERIOD_US = 20000;

    // ===== システムクロック =====
    // STM32F732RE の動作クロック周波数
    // PLL 設定: PLLN=216, PLLM=8 → 216MHz
    constexpr uint32_t SYSTEM_CLOCK_HZ = 216000000;

} // namespace SystemConfig

#endif // SYSTEM_CONFIG_HPP
