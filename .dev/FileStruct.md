```
PFLIGHT26/
├── Core/
│   ├── Inc/
│   │   ├── dma.h
│   │   ├── gpio.h
│   │   ├── main.h
│   │   ├── spi.h
│   │   ├── stm32f7xx_hal_conf.h
│   │   ├── stm32f7xx_it.h
│   │   ├── tim.h
│   │   ├── usart.h
│   │   └── wrapper.hpp
│   ├── Src/
│   │   ├── dma.c
│   │   ├── gpio.c
│   │   ├── main.c
│   │   ├── spi.c
│   │   ├── stm32f7xx_hal_msp.c
│   │   ├── stm32f7xx_it.c
│   │   ├── syscalls.c
│   │   ├── sysmem.c
│   │   ├── system_stm32f7xx.c
│   │   ├── tim.c
│   │   ├── usart.c
│   │   └── wrapper.cpp
│   ├── Startup/
│   │   └── startup_stm32f732retx.s
│   ├── StateManager/ 
│       ├── StateManager/
│       │   ├── state_manager.hpp
│       │   └── state_manager.cpp
│       ├── LoopManager/
│       │   ├── loop_manager.hpp    
│       ├── StateContext/
│       │   ├── context.hpp     
│       ├── StateFactory/
│       │   ├── state_factory.hpp
│       ├── StateInterface/
│       │   ├── state_interface.hpp
│       │   ├── state_id.hpp
│       │   └── state_result.hpp               
│       ├── StateBase/
│       │   ├── state_base.hpp
│       │   ├── init_state_base.cpp
│       │   ├── pre_flight_state_base.cpp
│       │   ├── manual_flight_state_base.cpp
│       │   ├── auto_flight_state_base.cpp
│       │   └── post_flight_state_base.cpp
│       └── States/
│           ├── StateHeaders.hpp
│           ├── InitStateBase/
│           │   └── init_state.cpp
│           ├── PreFlightStateBase/
│           │   ├── calibration_state.cpp
│           │   └── pre_flight_state.cpp
│           ├── ManualFlightStateBase/
│           │   ├── manual_flight_state.cpp
│           │   ├── level_turn_state.cpp
│           │   ├── climbing_turn_state.cpp
│           │   └── fugue_eight_state.cpp
│           ├── AutoFlightStateBase/
│           │   ├── pre_auto_flight_state.cpp
│           │   ├── auto_takeoff_state.cpp
│           │   ├── auto_flight_state.cpp
│           │   └── auto_landing_state.cpp
│           └── PostFlightStateBase/
│               ├── post_flight_state.cpp
│               └── emergency_state.cpp
├── Lib/
│   ├── STM32_Motor-Servo_Driver/
│   ├── STM32_ICM42688P/
│   ├── STM32_BMM350/
│   ├── SBUS/
│   ├── STM32_DPS368/
│   ├── 1DoF_PID/
│   ├── InertialEstimators/
│   └── MatLib/
```