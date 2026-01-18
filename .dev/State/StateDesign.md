## 状態一覧

| StateBase | 内容 |
|--------|--------|
|InitStateBase|特にやることない|
|PreFlightStateBase|特にやることない|
|ManualFlightStateBase|手動飛行の共通処理|
|AutoFlightStateBase|自動飛行の共通処理|
|PostFlightStateBase|飛行終了後の共通処理|

| 状態ID | StateBase | 状態名 | 内容 | 
|--------|--------|------|------|
| ST-1  | InitStateBase | InitState | 初期化・通信チェック |
| ST-2  | PreFlightStateBase | CalibrationState | センサーキャリブレーション |
| ST-3  | PreFlightStateBase | PreFlightState | 飛行待機 |
| ST-4  | ManualFlightStateBase | ManualFlightState | 手動飛行 |
| ST-5  | ManualFlightStateBase | LevelTurnState | 水平旋回 |
| ST-6  | ManualFlightStateBase | ClimbingTurnState | 上昇旋回 |
| ST-7  | ManualFlightStateBase | FugueEightState | 8の字飛行 |
| ST-8  | AutoFlightStateBase | PreAutoFlightState | 自動飛行（スタート） |
| ST-9  | AutoFlightStateBase | AutoTakeoffState | 自動離陸 |
| ST-10 | AutoFlightStateBase | AutoFlightState | 自動飛行 |
| ST-11 | AutoFlightStateBase | AutoLandingState | 自動着陸 |
| ST-12 | PostFlightStateBase | PostFlightState | 飛行終了後処理 |
| ST-13 | PostFlightStateBase | EmergencyState | 緊急事態対応 |

## 実装について

StateBaseクラスを継承しているため

- OnEnter(), OnExit(), OnUpdate()の各純粋仮想関数を実装する必要がある。
- GetStateID()も各状態で実装する必要がある。
- GetStateBaseID()はStateBaseクラスで実装されているため、派生クラスでの実装は不要。
