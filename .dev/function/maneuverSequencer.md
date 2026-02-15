役割: 現在の時間を入れると、キーフレームの線形補間を用いて現在時刻での目標値を提供する

D:\GitLocal\STM32CubeIDE\PFLIGHT_26\.dev\coding_rules_cpp.md
コーディングルールはこれを必ず守ってください


## データ構造

### ManualFlight

```cpp
uint32_t current_time_ms

float target_roll_deg
float target_pitch_deg
float target_yaw_deg

float target_altitude_m
```

### AutoFlight
今は実装しないが、将来的にはAutoFlightも実装する予定
実装ができる拡張性を持たせてほしい
```cpp
uint32_t current_time_ms

float target_roll_deg
float target_pitch_deg
float target_yaw_deg

float target_altitude_m

float target_x
float target_y
float target_z
```

### KeyFrame
これを構造体の羅列として登録することができる
```cpp
uint32_t time_ms

float roll_deg
float pitch_deg
float yaw_deg

float altitude_m
```

keyframeは簡略化のため、必ず時系列順に並んで配列に入っているとしてよい
ユーザーが調整するので、少し強引に縦のインデントをそろえる
```cpp
// keyframeの例    time, roll , pitch, yaw, altitude
KeyFrame frame1 = {0   , 0.0f , 0.0f, 0.0f, 0.0f};
KeyFrame frame2 = {1000, 10.0f, 0.0f, 0.0f, 1.0f};
KeyFrame frame3 = {2000, 20.0f, 0.0f, 0.0f, 2.0f};
KeyFrame frame4 = {3000, 30.0f, 0.0f, 0.0f, 3.0f};
KeyFrame keyFrames[] = {frame1, frame2, frame3, frame4};
```

```cpp
bool isMissionComplete(uint32_t current_time_ms, KeyFrame* keyFrames, int numKeyFrames) {
    // ここにミッション完了のロジックを実装する
    // current_time_msが最後のキーフレームの時間を超えている場合はtrueを返す
    // それ以外の場合はfalseを返す
    if (current_time_ms > keyFrames[numKeyFrames - 1].time_ms) {

        return true;
    }

    return false;
}

bool getTargetValues(uint32_t current_time_ms, KeyFrame* keyFrames, int numKeyFrames, float* target_roll_deg, float* target_pitch_deg, float* target_yaw_deg, float* target_altitude_m) {
    // ここに線形補間のロジックを実装する
    // current_time_msに基づいて、keyFramesから適切なフレームを見つけて、target_*に値を設定する
    // 例えば、current_time_msが1500msの場合、frame2とframe3の間に位置するので、これらのフレームを線形補間して目標値を計算する
    // current_time_msがkeyFramesの範囲外の場合は、最初または最後のフレームの値を返す
}

## 実装方法

実装場所はutitlity/maneuverSequencer.cppとする
keyflameのデータはMisson/[mission_name].cppに定義する

1. ManeuverSequencerクラスを作成し、その中に上記の関数を実装する
2. Misson/[mission_name].cppをStateManagerのstateidを参考に作成する
3. なんかいい感じにManeuverSequencerクラスと各keyframeを組み合わせる

## 質問（実装開始までに答えてください）

現状はMissonごとのkeyframeがたくさんある状態ですが、これをどのように指定して現在のミッションのデータを得ますか？
StateIDを使用するのは、デバック用のコードが書きにくくなるので、関数の引数やクラスの継承を利用するべきだと考えています