```
PFLIGHT2/
├── Core/
│   ├── Inc/
│   │   ├── main.h
│   │   └── wrapper.hpp
│   └── Src/
│       ├── main.c
│       └── wrapper.cpp
│
├── StateManager/
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
│   ├── ICM42688P/
│   ├── BMM350/
│   ├── DPS368/
│   ├── EKF/
│   ├── MotorServoDriver/
│   ├── WireLessConnection/
│   └── SBUS/
```