#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


ProcessStatus LevelFlightState::onUpdate(StateContext& context) {

    // ManeuverSequencer が初期化されているか確認
    if (!context.instances.maneuver_sequencer.has_value()) {
        printf("[LevelFlightState::onUpdate] ManeuverSequencer not initialized\n");
        return ProcessStatus::FAILURE;
    }

    ManeuverSequencer& sequencer = context.instances.maneuver_sequencer.value();

    // 初回のみミッション開始（MissionLevelFlight を使用）
    if (!mission_started_) {

        // MissionLevelFlight をスタック上に作成
        static MissionLevelFlight level_flight_mission;
        sequencer.startMission(&level_flight_mission);
        mission_started_ = true;

        printf("[LevelFlightState] Mission started - Level Flight\n");
    }

    // 目標値を取得
    float target_roll = 0.0f;
    float target_pitch = 0.0f;
    float target_yaw = 0.0f;
    float target_altitude = 0.0f;

    if (!sequencer.getTargetValues(target_roll, target_pitch, target_yaw, target_altitude)) {
        printf("[LevelFlightState::onUpdate] Failed to get target values\n");
        return ProcessStatus::FAILURE;
    }

    // カスケードPID制御の計算
    float pid_result[3] = {0.0f, 0.0f, 0.0f};  // [pitch, roll, yaw]

    if (!calculateCascadePID(context, target_pitch, target_roll, target_yaw, pid_result)) {
        printf("[LevelFlightState::onUpdate] Failed to calculate cascade PID\n");
        return ProcessStatus::FAILURE;
    }

    printf("pitch: %f, roll: %f, yaw: %f\n", pid_result[0], pid_result[1], pid_result[2]);

    // TODO: pid_result[0~2]を制御出力に変換してモーター・サーボに指令を送る

    return ProcessStatus::SUCCESS;
}


StateID LevelFlightState::evaluateNextState(StateContext& context) {

    // 安全スティックの値を確認（飛行終了判定）
    if(!context.rescaled_sbus_data.safety){

        return StateID::POST_FLIGHT_STATE;
    }

    // 手動飛行の判定
    if(!context.rescaled_sbus_data.flight_debug){

        return StateID::MANUAL_FLIGHT_STATE;
    }

    return StateID::LEVEL_FLIGHT_STATE;
}


StateID LevelFlightState::getStateID() const {

    return StateID::LEVEL_FLIGHT_STATE;
}


bool LevelFlightState::calculateCascadePID(StateContext& context, float target_pitch, float target_roll, float target_yaw, float pid_result[3]) {

    // PIDインスタンスの確認
    if (!context.instances.angle_pitch_pid.has_value() ||
        !context.instances.angle_roll_pid.has_value() ||
        !context.instances.angle_yaw_pid.has_value() ||
        !context.instances.rate_pitch_pid.has_value() ||
        !context.instances.rate_roll_pid.has_value() ||
        !context.instances.rate_yaw_pid.has_value()) {

        printf("[LevelFlightState::calculateCascadePID] PID instances not initialized\n");
        return false;
    }

    // 現在の姿勢状態を取得
    const AttitudeState& attitude = context.attitude_state;

    // ========== Pitch軸 ==========
    // 外側ループ（角度制御）: 目標角度 → 目標角速度
    PID& angle_pitch_pid = context.instances.angle_pitch_pid.value();
    angle_pitch_pid.calc(target_pitch, attitude.pitch);
    float target_pitch_rate = angle_pitch_pid.getData();

    // 内側ループ（角速度制御）: 目標角速度 → 制御出力
    PID& rate_pitch_pid = context.instances.rate_pitch_pid.value();
    rate_pitch_pid.calc(target_pitch_rate, attitude.pitch_rate);
    pid_result[0] = rate_pitch_pid.getData();

    // ========== Roll軸 ==========
    // 外側ループ（角度制御）: 目標角度 → 目標角速度
    PID& angle_roll_pid = context.instances.angle_roll_pid.value();
    angle_roll_pid.calc(target_roll, attitude.roll);
    float target_roll_rate = angle_roll_pid.getData();

    // 内側ループ（角速度制御）: 目標角速度 → 制御出力
    PID& rate_roll_pid = context.instances.rate_roll_pid.value();
    rate_roll_pid.calc(target_roll_rate, attitude.roll_rate);
    pid_result[1] = rate_roll_pid.getData();

    // ========== Yaw軸 ==========
    // 外側ループ（角度制御）: 目標角度 → 目標角速度
    PID& angle_yaw_pid = context.instances.angle_yaw_pid.value();
    angle_yaw_pid.calc(target_yaw, attitude.yaw);
    float target_yaw_rate = angle_yaw_pid.getData();

    // 内側ループ（角速度制御）: 目標角速度 → 制御出力
    PID& rate_yaw_pid = context.instances.rate_yaw_pid.value();
    rate_yaw_pid.calc(target_yaw_rate, attitude.yaw_rate);
    pid_result[2] = rate_yaw_pid.getData();

    return true;
}
