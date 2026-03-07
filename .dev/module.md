# このプログラムで使用しているモジュール

- モジュールは、参照やコンストラクタなどでCore/Configのファイルを読み取ることができる
- StateManagerのContextで1つずつインスタンスを保持する

## Core\Utility\Sensors

- データ取得、ekfをまとめたモジュール
- configをDIして、推定結果（角度+高度）データが得られる
- フォルダ外依存: 各センサーライブラリ、vector3f、euler3f

## Core\Utility\Motor_Servo

- モーターとサーボを簡単に動かすためのモジュール
- configを直接参照している
- フォルダ外依存: pwm_config,hpp

## Core\Utility\ManeuverSequencer

- 目標値供給システム
- ミッションごとにキーフレームを設定することができる
- フォルダ外依存: なし

## Core\Utility\PID

- カスケードPIDに関する処理をまとめたモジュール
- configを直接参照している