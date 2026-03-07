# AttitudeEstimation アーキテクチャ設計書

## 概要

姿勢推定（AttitudeEstimation）はセンサーから生データを読み込み、EKF・高度推定を実行し、姿勢情報を提供する中核コンポーネントです。本設計は以下の原則に基づいています：

- **認知負荷の最小化**: 各層の責務を明確に分離
- **クリーンな依存関係**: データが上から下へ流れる単方向フロー
- **シンプルなAPI**: 外部からの呼び出しは少数のメソッドのみ

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

