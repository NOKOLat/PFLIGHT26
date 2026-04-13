#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"
#include "../../../Utility/ManeuverSequencer/Missions/missions.hpp"
#include "../../../Utility/DebugPrinter/context_printer.hpp"


ProcessStatus LevelTurnState::onUpdate(StateContext& context) {

    // ManeuverSequencer が初期化されているか確認
    if (!context.instances.maneuver_sequencer.has_value()) {
        printf("[LevelTurnState::onUpdate] ManeuverSequencer not initialized\n");
        return ProcessStatus::FAILURE;
    }

    ManeuverSequencer& sequencer = context.instances.maneuver_sequencer.value();

    // 初回のみミッション開始（MissionLevelTurn を使用）
    if (!mission_started_) {

        // MissionLevelTurn をスタック上に作成
        static MissionLevelTurn level_turn_mission;
        sequencer.startMission(&level_turn_mission);
        mission_started_ = true;

        // 初期値オフセットを記録（相対的な目標値計算用）
        context.initial_yaw_offset = context.attitude.yaw();
        context.initial_altitude_offset = context.altitude;

        printf("[LevelTurnState] Mission started - Level Turn\n");
        printf("[LevelTurnState] Initial offsets - Yaw: %f, Altitude: %f\n",
               context.initial_yaw_offset, context.initial_altitude_offset);
    }

    // 目標値を取得（相対的な目標値）
    float target_roll = 0.0f;
    float target_pitch = 0.0f;
    float target_yaw = 0.0f;
    float target_altitude = 0.0f;

    if (!sequencer.getTargetValues(target_roll, target_pitch, target_yaw, target_altitude)) {
        printf("[LevelTurnState::onUpdate] Failed to get target values\n");
        return ProcessStatus::FAILURE;
    }

    // 初期値オフセットを適用（絶対的な目標値に変換）
    target_yaw += context.initial_yaw_offset;
    target_altitude += context.initial_altitude_offset;

    // カスケードPID制御の計算
    float pid_result[3] = {0.0f, 0.0f, 0.0f};  // [pitch, roll, yaw]

    target_roll -= 5.0f;

    // 全軸のカスケードPIDを計算
    const Euler3f& attitude = context.attitude;
    CascadePIDManager& pid_manager = context.instances.cascade_pid_manager.value();
    pid_manager.calcCascadePIDAllAxes(
        target_pitch, attitude.pitch(),
        target_roll, attitude.roll(),
        target_yaw, attitude.yaw(),
        pid_result
    );

    // Update cycle counter for rate inner loop frequency control
    pid_manager.updateCycleCounter();

    // PIDの値をサーボの角度に入力
    // pitch, rollのみを制御している
    // yawとスロットルはプロポからの入力を採用

    // 出力 = PID制御結果[deg] + サブトリム[deg]  clamp(±90°)
    // サブトリムはキャリブレーション値のMIDが標準中心(SBUS_MID=1024)からずれた分を角度換算した値
    const nokolat::SBUSRescaler::Thresholds& th = nokolat::SBUSRescaler::default_thresholds;
    context.control_output.servo_pwm.elevator()      = nokolat::SBUSRescaler::clamp(
        pid_result[0] + nokolat::SBUSRescaler::calcSubtrimAngle(th.elevator), -90.0f, 90.0f);       // pitch制御
    context.control_output.servo_pwm.rudder()        = context.rescaled_sbus_data.rudder * context.unit_conversion.SBUS_TO_SERVO_DEG;
    //context.control_output.servo_pwm.rudder()      = nokolat::SBUSRescaler::clamp(
    //    pid_result[2] + nokolat::SBUSRescaler::calcSubtrimAngle(th.rudder), -90.0f, 90.0f);       // yaw制御
    context.control_output.servo_pwm.left_aileron()  = nokolat::SBUSRescaler::clamp(
        pid_result[1] + nokolat::SBUSRescaler::calcSubtrimAngle(th.left_aileron), -90.0f, 90.0f);  // roll制御
    context.control_output.servo_pwm.right_aileron() = nokolat::SBUSRescaler::clamp(
        pid_result[1] + nokolat::SBUSRescaler::calcSubtrimAngle(th.right_aileron), -90.0f, 90.0f); // roll制御（左右同値）

    // デバック: サーボのデータ
    if(1){ ContextPrinter::printServo(context); }

    // エレベーターのリバースを適応
    context.control_output.servo_pwm.elevator() *= -1;


    // SBUSの値(0~100)をそのままスロットルの値(0~100)に入れる（ここは手動操縦）
    context.control_output.motor_pwm.right() = context.rescaled_sbus_data.throttle;
    context.control_output.motor_pwm.left()  = context.rescaled_sbus_data.throttle;


    if(1){ ContextPrinter::printPidResult(pid_result); }

    // TODO: pid_result[0~2]を制御出力に変換してモーター・サーボに指令を送る

    return ProcessStatus::SUCCESS;
}


StateID LevelTurnState::evaluateNextState(StateContext& context) {

    // 安全スティックの値を確認（飛行終了判定）
    if(context.rescaled_sbus_data.safety == SwitchPosition::LOW){

        return StateID::POST_FLIGHT_STATE;
    }

    // 手動飛行の判定
	if(context.rescaled_sbus_data.autofly == SwitchPosition::LOW){

	    return StateID::MANUAL_FLIGHT_STATE;
	}

	return StateID::LEVEL_TURN_STATE;
}


StateID LevelTurnState::getStateID() const {

    return StateID::LEVEL_TURN_STATE;
}
