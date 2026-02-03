#include "wrapper.hpp"
#include "optional"
#include "stdio.h"

#include "StateManager/state_manager.hpp"
#include "loop_manager.hpp"
#include "stm32f7xx_hal.h"
#include "isr_manager.hpp"

std::optional<StateManager> state_manager;
std::optional<LoopManager> loop_manager;
constexpr uint32_t loop_time_μs = 20000; //20ms

void init(){

	printf("Program Start Build: 1\n");

    loop_manager.emplace(loop_time_μs);
    state_manager.emplace(StateID::INIT_STATE, loop_time_μs);
}

void loop(){

    // 20msごとにStateManager::update()を呼び出す
    if (!loop_manager->isWaitNextLoop()) {

        state_manager->update();
    }
}

// UART受信完了コールバック関数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {

    // ISRマネージャに処理のみを委譲
    ISRManager::handleUartRxCplt(huart);
}
