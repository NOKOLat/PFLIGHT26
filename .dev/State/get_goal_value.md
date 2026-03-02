# 自動操縦目標値提供システム (ManeuverSequencer)

## 役割

現在の時刻を入力すると、キーフレームの線形補間を用いて自動操縦の目標値（ロール角、ピッチ角、ヨー角、高度）を提供するシステム。

---

## 設計決定

### 1. キーフレーム構造

時間と目標値をセットで管理する KeyFrame 構造体。

```cpp
struct KeyFrame {
    uint32_t time_ms;    // フレーム時刻 [ms]
    float    roll_deg;   // ロール目標値 [deg]
    float    pitch_deg;  // ピッチ目標値 [deg]
    float    yaw_deg;    // ヨー目標値 [deg]
    float    altitude_m; // 高度目標値 [m]
};
```

### 2. ミッション管理（継承によるポリモーフィズム）

StateID ではなく `MissionBase*` ポリモーフィズムでミッションを選択する。

- 利点：デバッグコードが書きやすい（ステートマシンに直結しないため）
- ミッション固有のロジック（キーフレーム管理）をミッションクラス自身で実装
- MissionBase は 2 つのメソッドを提供：
  - `getKeyFrames(int& num_frames)` - キーフレーム配列を返す
  - `getActiveChannels()` - このミッションが制御するチャンネルを示すビットマスクを返す

### 3. チャンネル管理（ミッション単位）

**なぜ必要か：** ミッションによって制御するチャンネルが異なる。例えば、高度制御を行わない「姿勢制御のみミッション」と「高度・姿勢両方制御するミッション」を区別したい。

**実装：** ChannelFlags ビットマスク
```cpp
namespace ChannelFlags {
    constexpr uint8_t ROLL     = 0x01;
    constexpr uint8_t PITCH    = 0x02;
    constexpr uint8_t YAW      = 0x04;
    constexpr uint8_t ALTITUDE = 0x08;
    constexpr uint8_t ATTITUDE = 0x07;  // ROLL | PITCH | YAW
    constexpr uint8_t ALL      = 0x0F;
}
```

各ミッションの `getActiveChannels()` でビットマスクを返す：
```cpp
// 例：高度制御を行わないミッション
uint8_t MissionAttitudeOnly::getActiveChannels() const {
    return ChannelFlags::ATTITUDE;
}

// 例：全チャンネル制御するミッション
uint8_t MissionFull::getActiveChannels() const {
    return ChannelFlags::ALL;
}
```

AutoFlightState は `getActiveChannels()` で有効チャンネルを確認してから PID コントローラに目標値を渡す。

### 4. 時刻管理（HAL_GetTick() ベース）

ManeuverSequencer はステートフル（状態を保持）：

- `startMission(mission)` でミッション開始時刻を `HAL_GetTick()` で記録
- `getTargetValues()` は `HAL_GetTick()` から開始時刻を引いた経過時間で補間計算
- 呼び出し側は経過時間を計算する必要なし（ManeuverSequencer が内部で管理）

### 5. ファイル分割（ミッション単位）

```
Core/Utility/ManeuverSequencer/
├── maneuver_sequencer.hpp    - ManeuverSequencer + KeyFrame
├── maneuver_sequencer.cpp    - 実装
└── Missions/
    ├── mission_base.hpp      - MissionBase インターフェース
    ├── mission_test.hpp      - テスト用ミッション（例：4フレーム）
    └── mission_test.cpp      - キーフレームデータ
```

各ミッションは以下のように定義：

**mission_test.hpp**
```cpp
class MissionTest : public MissionBase {
    public:
        const KeyFrame* getKeyFrames(int& num_frames) const override;
        uint8_t getActiveChannels() const override;
    private:
        static constexpr int NUM_KEY_FRAMES = 4;
        KeyFrame key_frames[NUM_KEY_FRAMES];
};
```

**mission_test.cpp**
```cpp
MissionTest::MissionTest() {
    // time_ms, roll_deg, pitch_deg, yaw_deg, altitude_m
    key_frames[0] = {0,    0.0f,  0.0f,  0.0f, 0.0f};
    key_frames[1] = {1000, 10.0f, 5.0f,  5.0f, 5.0f};
    key_frames[2] = {2000, 20.0f, 10.0f, 10.0f, 10.0f};
    key_frames[3] = {3000, 0.0f,  0.0f,  0.0f, 0.0f};
}

const KeyFrame* MissionTest::getKeyFrames(int& num_frames) const {
    num_frames = NUM_KEY_FRAMES;
    return key_frames;
}

uint8_t MissionTest::getActiveChannels() const {
    return ChannelFlags::ALL;  // 全チャンネル制御
}
```

---

## 統合方法

### StateContext への追加

```cpp
struct Instances {
    // ...既存インスタンス...
    std::optional<ManeuverSequencer> maneuver_sequencer;
};

struct StateContext {
    // ...既存フィールド...
    const MissionBase* current_mission = nullptr;  // PreAutoFlightState がセット
};
```

### StateManager での初期化

```cpp
void StateManager::init() {
    // ...既存の初期化...
    state_context_.instances.maneuver_sequencer.emplace();
    // ...
}
```

### AutoFlightState での使用

```cpp
class AutoFlightState : public AutoFlightStateBase {
    private:
        bool mission_started_ = false;  // 初回フラグ

    protected:
        ProcessStatus onUpdate(StateContext& context) override {
            // 初回のみミッション開始
            if (!mission_started_) {
                if (context.current_mission != nullptr) {
                    context.instances.maneuver_sequencer->startMission(context.current_mission);
                    mission_started_ = true;
                } else {
                    return ProcessStatus::FAILURE;
                }
            }

            // 目標値取得
            float target_roll, target_pitch, target_yaw, target_altitude;
            if (!context.instances.maneuver_sequencer->getTargetValues(
                    target_roll, target_pitch, target_yaw, target_altitude)) {
                return ProcessStatus::FAILURE;
            }

            // アクティブチャンネルを確認
            uint8_t active_ch = context.instances.maneuver_sequencer->getActiveChannels();

            // PID コントローラに目標値を渡す
            if (active_ch & ChannelFlags::ROLL) {
                context.instances.angle_roll_pid->setTarget(target_roll);
            }
            if (active_ch & ChannelFlags::PITCH) {
                context.instances.angle_pitch_pid->setTarget(target_pitch);
            }
            if (active_ch & ChannelFlags::YAW) {
                context.instances.angle_yaw_pid->setTarget(target_yaw);
            }
            if (active_ch & ChannelFlags::ALTITUDE) {
                // 高度制御ロジック（別途実装）
            }

            return ProcessStatus::SUCCESS;
        }

        StateID evaluateNextState(StateContext& context) override {
            // ミッション完了判定
            if (context.instances.maneuver_sequencer->isMissionComplete()) {
                return StateID::AUTO_LANDING_STATE;
            }
            return StateID::AUTO_FLIGHT_STATE;
        }

    public:
        StateID getStateID() const override {
            return StateID::AUTO_FLIGHT_STATE;
        }
};
```

---

## 線形補間アルゴリズム

2 つのキーフレーム間を線形補間：

```
value(t) = value_a + (t - time_a) / (time_b - time_a) * (value_b - value_a)
```

- `t` は現在時刻 [ms]
- 現在時刻がキーフレーム範囲外の場合は端点値を返す
- 線形探索でキーフレームペア(a, b)を見つけて補間

---

## まとめ

| 項目 | 値 |
|---|---|
| **時刻源** | HAL_GetTick() [ms] |
| **補間方法** | 線形補間 |
| **ミッション選択方式** | MissionBase* ポリモーフィズム |
| **チャンネル管理** | ビットマスク（ChannelFlags） |
| **ファイル分割** | ミッション単位（Missions/ ディレクトリ） |
| **状態管理** | ManeuverSequencer がステートフル |
