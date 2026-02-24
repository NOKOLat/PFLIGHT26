# 設定管理ガイド

プロジェクトのすべての設定項目は `Core/Config/` フォルダに一元管理されています。

## Config ファイル一覧

| ファイル | 用途 | 主な内容 |
|---------|------|--------|
| `board_config.hpp` | ハードウェアピン設定 | I2C/UART/TIM ハンドルと TIM チャンネル割り当て |
| `pwm_config.hpp` | PWM パルス幅設定 | モーター・サーボの パルス幅上下限 |
| `pid_config.hpp` | PID ゲイン初期値 | 角度制御・角速度制御の P, I, D 値 |
| `sbus_config.hpp` | SBUS チャンネルマッピング | チャンネル番号とスイッチ閾値 |
| `sensor_i2c_config.hpp` | センサー I2C アドレス | IMU (0x69), 地磁気 (0x14), 気圧 (0x77) |
| `system_config.hpp` | システムパラメータ | メインループ周期 (20ms), システムクロック |

---

## 設定値の変更方法

### パルス幅を変更したい
→ `pwm_config.hpp` を編集
```cpp
constexpr uint32_t SERVO_MIN_PULSE_US = 1000;  // -90度
```

### センサーの I2C アドレスを変更したい
→ `sensor_i2c_config.hpp` を編集
```cpp
constexpr uint8_t DPS368_ADDR = 0x77;  // PSAピンで 0x76 に変更可能
```

### PID ゲインをチューニングしたい
→ `pid_config.hpp` を編集
```cpp
constexpr float ANGLE_KP = 1.0f;
```

### メインループ周期を変更したい
→ `system_config.hpp` を編集
```cpp
constexpr uint32_t MAIN_LOOP_PERIOD_US = 20000;  // 20ms = 50Hz
```

### SBUS チャンネルマッピングを変更したい
→ `sbus_config.hpp` を編集
```cpp
constexpr uint8_t CH_THROTTLE = 2;  // ch3
```

### ピン/TIM/チャンネル割り当てを変更したい
→ `board_config.hpp` を編集
```cpp
inline TIM_HandleTypeDef* const right_motor_tim = &htim3;
constexpr uint32_t RIGHT_MOTOR_CHANNEL = TIM_CHANNEL_1;
```

---

## 設定が反映される箇所

| Config ファイル | 参照元ファイル |
|---------|---------|
| `board_config.hpp` | `Pwm.hpp`, `context.hpp`, `SensorManager.cpp`, `DPS368_HAL_I2C.hpp` |
| `pwm_config.hpp` | `Pwm.hpp` |
| `pid_config.hpp` | `context.hpp` |
| `sbus_config.hpp` | `sbus_rescaler.hpp` |
| `sensor_i2c_config.hpp` | `SensorManager.cpp`, `DPS368_HAL_I2C.cpp/hpp` |
| `system_config.hpp` | `wrapper.cpp` |
