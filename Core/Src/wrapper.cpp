#include "wrapper.hpp"
#include "optional"
#include "stdio.h"

#include "StateManager/state_manager.hpp"
#include "loop_manager.hpp"
#include "stm32f7xx_hal.h"

std::optional<StateManager> state_manager;
std::optional<LoopManager> loop_manager;

void init(){

	printf("Program Start Build: 1\n");

    loop_manager.emplace(20000); // 20ms
    state_manager.emplace(StateID::INIT_STATE);
}

void loop(){

    // 20msごとにStateManager::update()を呼び出す
    if (!loop_manager->isWaitNextLoop()) {

        state_manager->update();
    }
}
