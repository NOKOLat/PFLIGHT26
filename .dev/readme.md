# ミッション



### 実行環境
- STM32CubeIDE
- STM32F732

### 使用センサー
- ICM42688P (6軸IMU)
- BMM350 (3軸磁気センサー)

### ファイル構成（改善版）

#### 推奨構造
```
PFLIGHT2/
├── Core/
│   ├── Inc/
│   │   ├── main.h
│   │   └── wrapper.hpp
│   ├── Src/
│   │   ├── main.c
│   │   └── wrapper.cpp
│   └── StateManager/
│       ├── StateManager/
│       │   ├── state_manager.hpp
│       │   └── state_manager.cpp
│       ├── StateContext/
│       │   ├── context.hpp     
│       ├── StateFactory/
│       │   ├── state_factory.hpp
│       ├── StateInterface/
│       │   ├── state_interface.hpp               
│       ├── StateBase/
│       │   ├── state_base.hpp
│       │   ├── init_state_base.cpp
│       │   ├── pre_flight_state_base.cpp
│       │   ├── manual_flight_state_base.cpp
│       │   ├── auto_flight_state_base.cpp
│       │   └── post_flight_state_base.cpp
│       ├── LoopManager/
│       │   └── loop_manager.hpp
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
│   │   ├── imu.hpp
│   │   └── imu.cpp
│   ├── BMM350/
│   │   ├── magnetometer.hpp
│   │   └── magnetometer.cpp
│   ├── MotorServoDriver/
│   ├── WireLessConnection/
│   └── SBUS/
```