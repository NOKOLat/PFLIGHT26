## ファイル構造について

このプロジェクトではこのようなファイル構造を採用しています

### 参考資料
- [StateBaseとは](Docs/StateBase.md)
- [設定方法](Docs/Config.md)


```
PFLIGHT_26/
├── Core/
│   ├── Config/                              # システム設定ファイル
│   │   ├── board_config.hpp                 # ボード設定
│   │   ├── pid_config.hpp                   # PID制御設定
│   │   ├── pwm_config.hpp                   # PWM設定
│   │   ├── sbus_config.hpp                  # SBUS通信設定
│   │   ├── sensor_i2c_config.hpp            # I2Cセンサ設定
│   │   └── system_config.hpp                # システム全般設定
│   │
│   ├── Inc/                                 # メインヘッダー
│   │   ├── main.h
│   │   ├── wrapper.hpp                      # ハードウェア抽象化レイヤー
│   │   ├── isr_manager.hpp                  # 割り込みハンドラ管理
│   │   └── loop_manager.hpp                 # メインループ管理
│   │
│   ├── Src/                                 # メイン実装
│   │   ├── main.c
│   │   ├── wrapper.cpp
│   │   ├── isr_manager.cpp
│   │   └── sbus_rescaler.cpp                # SBUS信号スケーリング
│   │
│   ├── Lib/                                 # 外部ライブラリ・ドライバ
│   │   ├── 1DoF_PID/                        # PID制御ライブラリ
│   │   ├── MadgwickAHRS/                    # 姿勢推定アルゴリズム
│   │   ├── SBUS/                            # SBUS通信ライブラリ
│   │   ├── STM32_BMM350/                    # 磁力計ドライバ
│   │   ├── STM32_DPS368/                    # 気圧計・温度計ドライバ
│   │   ├── STM32_ICM42688P/                 # IMU（加速度・ジャイロ）ドライバ
│   │   └── STM32_Motor-Servo_Driver/        # モーター・サーボドライバ
│   │
│   ├── Utility/                             # ユーティリティ・高レベル機能
│   │   ├── Vector3f.hpp                     # 3次元ベクトル演算
│   │   │
│   │   ├── Sensors/                         # センサ管理・統合
│   │   │   ├── SensorManager.hpp
│   │   │   └── SensorManager.cpp
│   │   │
│   │   ├── Motor_Servo/                     # モーター・サーボ制御
│   │   │   ├── Pwm.hpp
│   │   │   └── Pwm.cpp
│   │   │
│   │   └── ManeuverSequencer/               # 飛行マニューバ計画
│   │       ├── maneuver_sequencer.hpp
│   │       ├── maneuver_sequencer.cpp
│   │       └── missions/                    # ミッション定義
│   │
│   └── StateManager/                        # 状態管理システム
│       ├── StateManager/
│       │   ├── state_manager.hpp
│       │   └── state_manager.cpp
│       │
│       ├── StateContext/
│       │   └── context.hpp
│       │
│       ├── StateFactory/
│       │   └── state_factory.hpp
│       │
│       ├── StateInterface/
│       │   └── state_interface.hpp
│       │
│       ├── StateBase/                       # 各フェーズの基底クラス
│       │   ├── state_base.hpp
│       │   ├── init_state_base.cpp
│       │   ├── pre_flight_state_base.cpp
│       │   ├── manual_flight_state_base.cpp
│       │   ├── auto_flight_state_base.cpp
│       │   └── post_flight_state_base.cpp
│       │
│       └── States/                          # 具体的な状態実装
│           ├── StateHeaders.hpp
│           │
│           ├── InitStateBase/
│           │   └── init_state.cpp
│           │
│           ├── PreFlightStateBase/
│           │   ├── calibration_state.cpp
│           │   ├── pre_flight_state.cpp
│           │   ├── sensor_test_state.cpp
│           │   └── servo_motor_test_state.cpp
│           │
│           ├── ManualFlightStateBase/
│           │   ├── manual_flight_state.cpp
│           │   ├── level_flight_state.cpp
│           │   ├── level_turn_state.cpp
│           │   ├── climbing_turn_state.cpp
│           │   └── fugue_eight_state.cpp
│           │
│           ├── AutoFlightStateBase/
│           │   ├── pre_auto_flight_state.cpp
│           │   ├── auto_takeoff_state.cpp
│           │   ├── auto_flight_state.cpp
│           │   └── auto_landing_state.cpp
│           │
│           └── PostFlightStateBase/
│               ├── post_flight_state.cpp
│               └── emergency_state.cpp
│
├── Drivers/                                 # STM32 HALドライバ（自動生成）
│   ├── CMSIS/
│   └── STM32F7xx_HAL_Driver/
│
└── Docs/                                    # ドキュメント
    ├── fliestruct.md                        # このファイル（ファイル構造）
    ├── StateBase.md                         # 状態管理の説明
    └── Config.md                            # 設定方法
```