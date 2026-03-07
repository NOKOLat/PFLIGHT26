# AttitudeEstimation アーキテクチャ設計書

## 概要

姿勢推定（AttitudeEstimation）はセンサーから生データを読み込み、EKF・高度推定を実行し、姿勢情報を提供する中核コンポーネントです。本設計は以下の原則に基づいています：

- **認知負荷の最小化**: 各層の責務を明確に分離
- **クリーンな依存関係**: データが上から下へ流れる単方向フロー
- **シンプルなAPI**: 外部からの呼び出しは少数のメソッドのみ

各センサーライブラリをまとめた、SensorManager（自作）と姿勢推定ライブラリを統合したものが
AttitudeEstimationクラスになっています

---

## アーキテクチャ図

```
┌─────────────────────────────────────────────────────────────────┐
│ StateManager（最上位・ファサード層）                              │
│ ├─ Config 読み込み                                                │
│ ├─ SensorConfig 生成                                              │
│ └─ AttitudeEstimation へ DI（依存性注入）                        │
└─────────────────────────────────────────────────────────────────┘
                              ↓ DI
┌─────────────────────────────────────────────────────────────────┐
│ AttitudeEstimation（センサー・推定・フィルタの統合）              │
│                                                                   │
│  initialize(SensorConfig)                                        │
│  ├─ SensorManager 生成（I2C_HandleTypeDef で初期化）            │
│  ├─ AttitudeEKF 初期化                                          │
│  └─ Altitude（高度推定） 初期化                                  │
│                                                                   │
│  update(dt) ← State から呼び出すのはこれだけ                    │
│  ├─ SensorManager.read() → SensorRawData                        │
│  ├─ EKF.update(raw_data) → 角度計算                            │
│  ├─ Altitude.update(pressure) → 高度計算                       │
│  └─ MovingAverage.update() → フィルタリング                    │
│                                                                   │
│  getAttitudeState() → AttitudeState 結果を返す                 │
└─────────────────────────────────────────────────────────────────┘
                              ↓ 内部 ↓
        ┌─────────────────┬──────────┬──────────────┐
        │                 │          │              │
   ┌────▼─────┐  ┌───────▼──┐ ┌────▼──────┐ ┌────▼─────────┐
   │SensorMgr │  │AttEKF    │ │Altitude   │ │MovingAverage│
   ├──────────┤  ├──────────┤ ├───────────┤ ├──────────────┤
   │ICM42688P │  │Attitude  │ │Kalman     │ │yaw_avg      │
   │BMM350    │  │Estimate  │ │Filter     │ │altitude_avg │
   │DPS368    │  │          │ │           │ │              │
   └──────────┘  └──────────┘ └───────────┘ └──────────────┘
        ↓              ↓            ↓
   [I2C BUS]    [計算結果]    [推定高度]
```

---

## 依存関係フロー

### 初期化時

```
StateManager::init()
    ↓
1. SensorConfig を準備（Config から読み込み）
    {
        .i2c_handle = state_context_.pin_config.sensor_i2c,
        .enable_imu = true,
        .enable_magnetometer = true,
        .enable_barometer = true
    }
    ↓
2. AttitudeEstimation.initialize(sensor_config) ← DI
    ↓
3. 内部で SensorManager 生成
    └─ I2C_HandleTypeDef から各センサードライバを初期化
    ↓
4. EKF / Altitude / MovingAverage を初期化
    ↓
[Ready]
```

### 実行時（メインループ）

```
State::onUpdate(context)
    ↓
context.attitude_estimation.update(dt) ← シンプルな1行呼び出し
    ↓ 内部処理（State からは見えない）
    ├─ SensorManager.read()
    │  ├─ ICM42688P (I2C) → accel, gyro
    │  ├─ BMM350 (I2C) → mag
    │  └─ DPS368 (I2C) → pressure, temperature
    │  ↓
    ├─ EKF.update(accel, gyro) → roll, pitch, yaw 計算
    │
    ├─ Altitude.update(pressure) → 高度計算
    │
    └─ MovingAverage.update() → yaw_avg, altitude_avg
    ↓
context.attitude_state = context.attitude_estimation.getAttitudeState()
    ↓
[State が姿勢情報を使用]
```

---

## クラス責務の明確化

### StateManager（最上位）

| 責務 | 詳細 |
|------|------|
| **複雑性の吸収** | 初期化処理を全て集約 |
| **Config管理** | SensorConfig を生成 |
| **DI実施** | AttitudeEstimation へ依存性を注入 |
| **ファサード** | State 層からは細部を隠蔽 |

**依存先:**
- Config（board_config, sensor_config）
- AttitudeEstimation
- ISRManager
- StateFactory

---

### AttitudeEstimation（中核）

| 責務 | 詳細 |
|------|------|
| **センサー管理** | SensorManager を所有 |
| **データ処理パイプライン** | 生データ → 推定値への変換 |
| **フィルタリング** | MovingAverage で平滑化 |
| **結果提供** | AttitudeState を公開 |

**依存先:**
- SensorConfig（初期化パラメータ）
- SensorManager（センサー読み込み）
- AttitudeEKF（角度推定）
- Altitude（高度推定）
- MovingAverage<float>（フィルタ）

**公開インターフェース:**
```cpp
bool initialize(const SensorConfig& config);  // 初期化
bool update(float dt);                        // メイン処理
const AttitudeState& getAttitudeState() const; // 結果取得
```

---

### SensorManager（センサー層）

| 責務 | 詳細 |
|------|------|
| **センサー統合** | 複数センサーを1つのインターフェースで提供 |
| **低レイヤー管理** | I2C通信、ドライバ呼び出し |
| **キャリブレーション** | センサーオフセット管理 |

**依存先:**
- I2C_HandleTypeDef（通信ハンドル）
- ICM42688P_HAL_SPI（IMU）
- MockBMM350（磁気）
- DPS368_HAL_I2C（気圧/温度）

**公開インターフェース:**
```cpp
bool initSensors();
bool updateSensors();
bool getAccelData(Vector3f* accel);
bool getGyroData(Vector3f* gyro);
bool getMagData(Vector3f* mag);
bool getPressData(float* press);
bool getTempData(float* temp);
```

---

## 依存関係グラフ

```
最上位層（認知負荷高）
    ↓
┌─ StateManager
│      ↓ DI
│   ┌─ SensorConfig（設定値のみ）
│   │
│   └─ AttitudeEstimation
│        ├─ SensorManager
│        │  ├─ ICM42688P
│        │  ├─ BMM350
│        │  ├─ DPS368
│        │  └─ I2C通信層
│        │
│        ├─ AttitudeEKF（内部アルゴリズム）
│        ├─ Altitude（内部アルゴリズム）
│        └─ MovingAverage<float>（フィルタ）
│
└─ State（ManualFlightState, SensorTestState）
    └─ attitude_estimation.update(dt) ← シンプル
        └─ getAttitudeState() ← 結果取得

最下位層（認知負荷低）
```

**特徴:**
- ✅ **単方向性**: 上層が下層に依存、下層は上層に依存しない
- ✅ **明確な責務境界**: 各層が独立した関心事を管理
- ✅ **テスト可能性**: SensorConfig による DI で Mock 挿入容易
- ✅ **変更の影響を最小化**: 設定変更は StateManager のみ対応

---

## データ構造

### SensorConfig（依存性注入の契約）
```cpp
struct SensorConfig {
    I2C_HandleTypeDef* i2c_handle;     // 通信ハンドル
    bool enable_imu;                   // センサー有効フラグ
    bool enable_magnetometer;
    bool enable_barometer;
};
```

### SensorRawData（センサー生データ）
```cpp
struct SensorRawData {
    Vector3f accel;              // 加速度 [m/s^2]
    Vector3f gyro;               // 角速度 [rad/s]
    Vector3f mag;                // 磁気 [uT]
    float barometric_pressure;   // 気圧 [Pa]
    float temperature;           // 温度 [℃]
};
```

### AttitudeState（推定結果）
```cpp
struct AttitudeState {
    Euler3f angle;               // ロール・ピッチ・ヨー [deg]
    Euler3f rate;                // ロール・ピッチ・ヨー角速度 [deg/s]
    float yaw_avg;               // ヨー角平均 [deg]
    float altitude;              // 高度 [m]
    float altitude_avg;          // 高度平均 [m]
};
```

---

## API 使用例

### 初期化（StateManager 内）
```cpp
// SensorConfig を準備
SensorConfig sensor_config = {
    .i2c_handle = state_context_.pin_config.sensor_i2c,
    .enable_imu = true,
    .enable_magnetometer = true,
    .enable_barometer = true
};

// 初期化（内部で SensorManager を生成）
state_context_.attitude_estimation.initialize(sensor_config);
```

### 実行（State 内）
```cpp
// State では このたった2行だけで完結
float dt = context.loop_time_us / 1000000.0f;
context.attitude_estimation.update(dt);
context.attitude_state = context.attitude_estimation.getAttitudeState();
```

---

## 設計原則

### 1. 認知負荷の最小化
- **State**: `update()` と `getAttitudeState()` だけ覚えればよい
- **内部処理**: すべて AttitudeEstimation 内にカプセル化

### 2. 単方向な依存フロー
```
Config → SensorConfig → AttitudeEstimation → State
        ↑              ↑                  ↑
    準備            受け取り             使用
```
**逆方向の依存はない** ← テスト・保守性向上

### 3. ファサード層による複雑性の吸収
- StateManager が初期化の複雑性をすべて隠蔽
- State から見ると、AttitudeEstimation は単純なブラックボックス

### 4. DI による柔軟性
- SensorConfig で センサー構成を指定可能
- 将来的に異なるセンサーセットへの対応が容易

---

## 将来の拡張性

### Phase 2: センサーの抽象化
```cpp
// ISensor インターフェース化
class ISensor {
    virtual bool initialize() = 0;
    virtual bool read() = 0;
};

class IMUSensor : public ISensor { };
class MagnetometerSensor : public ISensor { };
```
→ テスト時に Mock センサーを注入可能

### Phase 3: 推定アルゴリズムの切り替え
```cpp
// IAttitudeEstimator インターフェース化
class IAttitudeEstimator {
    virtual bool update(const SensorRawData& data) = 0;
};

// EKF / Complementary Filter / その他を選択可能
```

---

## トラブルシューティング

| 現象 | 原因 | 解決方法 |
|------|------|---------|
| `initialize()` が失敗 | I2C ハンドル が null | StateManager で SensorConfig を正しく生成 |
| `update()` でセンサー読み込み失敗 | I2C 通信エラー | SensorManager の initSensors() を確認 |
| 姿勢データがおかしい | EKF が発散 | AttitudeEKF_Init() のパラメータを確認 |
| 高度がジッター | フィルター不足 | MovingAverage のウィンドウサイズを増加 |


### 想定している実装
- 実際の実装では名前空間の代わりにファイルがわかれていたり、クラスになっていたりします
```cpp
#include "Inc/wrapper.hpp"

#include "Inc/i2c.h"
#include "stdio.h"
#include "AttitudeEstimation/attitude_estimation.hpp"

namespace {

	constexpr float ATTITUDE_ESTIMATION_DT = 0.01f;
	constexpr uint32_t ATTITUDE_ESTIMATION_LOG_INTERVAL = 10;

	AttitudeEstimation g_attitude_estimation;

	SensorConfig createDefaultSensorConfig() {
		SensorConfig config;

		// 1. I2C ハンドルを設定
		config.i2c_handle = &hi2c1;

		// 2. I2C アドレスを設定
		config.i2c_addresses.icm42688p_addr = 0x69;
		config.i2c_addresses.bmm350_addr = 0x14;
		config.i2c_addresses.dps368_addr = 0x77;

		// 3. 使用するセンサーを有効化
		config.enable_imu = true;
		config.enable_magnetometer = true;
		config.enable_barometer = true;

		// 4. IMU キャリブレーション設定
		config.imu_calib.enable_auto_calibration = false;
		config.imu_calib.manual_accel_offset[0] = -286;
		config.imu_calib.manual_accel_offset[1] = -277;
		config.imu_calib.manual_accel_offset[2] = 0;
		config.imu_calib.manual_gyro_offset[0] = -5;
		config.imu_calib.manual_gyro_offset[1] = -8;
		config.imu_calib.manual_gyro_offset[2] = -37;

		// 5. 高度推定キャリブレーション設定
		config.altitude_calib.sample_count = 10;
		config.altitude_calib.reference_gravity = 9.80665f;

		return config;
	}

}

void init() {
	printf("Initializing sensor system...\r\n");

	const SensorConfig config = createDefaultSensorConfig();

	if (g_attitude_estimation.initialize(config)) {
		printf("Sensor system initialized successfully\r\n");
	}
	else {
		printf("Failed to initialize sensor system\r\n");
	}
}

void loop() {
	static uint32_t last_time = 0;
	static uint32_t log_count = 0;
	static bool is_calib_logged = false;

	const uint32_t current_time = HAL_GetTick();
	const uint32_t target_ms = static_cast<uint32_t>(ATTITUDE_ESTIMATION_DT * 1000.0f);

	if (current_time - last_time < target_ms) {
		return;
	}

	last_time = current_time;

	if (!g_attitude_estimation.update(ATTITUDE_ESTIMATION_DT)) {
		printf("[ERROR] Sensor update failed\r\n");
		return;
	}

	const AttitudeState& state = g_attitude_estimation.getAttitudeState();

	if (log_count++ % ATTITUDE_ESTIMATION_LOG_INTERVAL == 0) {
		printf(
			"Attitude - Roll: %.2f, Pitch: %.2f, Yaw: %.2f, Altitude: %.2f m\r\n",
			state.angle.roll(),
			state.angle.pitch(),
			state.angle.yaw(),
			state.altitude
		);
	}

	if (g_attitude_estimation.isAltitudeCalibrated() && !is_calib_logged) {
		printf("[INFO] Altitude calibration completed\r\n");
		is_calib_logged = true;
	}
}
```