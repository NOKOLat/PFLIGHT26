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

    // 20msごとにStateManager::update()を呼び出す
    if (!loop_manager->isWaitNextLoop()) {

        state_manager->update();
    }
}

// UART受信イベントコールバック関数（アイドルライン検出版）
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {

    // ISRマネージャに処理を委譲
    ISRManager::handleUartRxEvent(huart, Size);
}
