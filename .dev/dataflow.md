## SBUS

初期化はStateManagerのinit()で行う。UART5をSBUS受信用に設定し、UART5Callbackを登録する。

### wrapper.cpp
1. UART5Callback

### ISRManager.cpp
2. ISRManagerで処理を受け取る
3. SBUSクラスでデータを処理してuin16_t[18]の形式で保存

### StateManager.cpp
4. StateManagerのupdate()で、SBUSクラスから最新のデータを取得する
5. SbusResclaerクラスで、スロットル[0~100%]、動翼[-90~90度]、スイッチ[ON/OFF]の形式に変換する
6. StateContextに保存する

## 姿勢推定

初期化はStateManagerのinit()で行う。AttitudeEstimationクラスのインスタンスを作成し、必要なパラメータを設定する。

### SensorManager.cpp

- Configから受け取ったピン情報をもとにセンサーを初期化する
- AttitudeEstimationクラスに所有されていて、センサーデータを提供する役割を担う

### State.cpp
1. AttitudeEstimationクラスのupdate()を呼び出す

### AttitudeEstimation.cpp
2. update()内で、センサーデータを用いて姿勢推定を行う

### State.cpp
3. 推定結果をStateContextのattitude_stateに保存する
