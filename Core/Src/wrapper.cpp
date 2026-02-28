#include "wrapper.hpp"
#include "optional"
#include "stdio.h"

#include "StateManager/state_manager.hpp"
#include "loop_manager.hpp"
#include "isr_manager.hpp"

#include "../Config/system_config.hpp"

std::optional<StateManager> state_manager;
std::optional<LoopManager> loop_manager;
constexpr uint32_t loop_time_μs = SystemConfig::MAIN_LOOP_PERIOD_US;

void init(){

    loop_manager.emplace(loop_time_μs);
    state_manager.emplace(StateID::INIT_STATE, loop_time_μs);
}

void loop(){

    static bool just_ran = false;

    // 20msごとにStateManager::update()を呼び出す
    if (!loop_manager->isWaitNextLoop()) {

        // 前回のupdate()完了後にwait状態を経ずに再び実行 → オーバーラン
        if (just_ran) {
            printf("[WARN] update() overrun: did not complete within %lu us\r\n", loop_time_μs);
        }

        just_ran = true;
        state_manager->update();

    } else {
        // 待機中 = 正常なウェイト状態
        just_ran = false;
    }
}

// UART受信イベントコールバック関数（アイドルライン検出版）
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {

    // ISRマネージャに処理を委譲
    ISRManager::handleUartRxEvent(huart, Size);
}
