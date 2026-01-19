#include "wrapper.hpp"
#include "tim.h"
#include "optional.h"
#include "stdio.h"

#include "state_manager.hpp"
#include "loop_manager.hpp"

std::optional<StateManager*> state_manager;
std::optional<LoopManager*> loop_manager;

void init(){

    loop_manager.emplace(20000); // 20ms
    state_manager.emplace(StateID::INIT_STATE);
}

void loop(){

    // 20msごとにStateManager::update()を呼び出す
    if (!loop_manager->isWaitNextLoop()) {

        state_manager->update();
    }
}