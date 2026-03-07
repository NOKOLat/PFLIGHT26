#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"
#include "../../Utility/PID/cascade_pid_controller.hpp"


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

        // 初期値オフセットを記録（相対的な目標値計算用）
        context.initial_yaw_offset = context.attitude_state.yaw_avg;
        context.initial_altitude_offset = context.attitude_state.altitude_avg;

        printf("[LevelFlightState] Mission started - Level Flight\n");
        printf("[LevelFlightState] Initial offsets - Yaw: %f, Altitude: %f\n",
               context.initial_yaw_offset, context.initial_altitude_offset);
    }

    // 目標値を取得（相対的な目標値）
    float target_roll = 0.0f;
    float target_pitch = 0.0f;
    float target_yaw = 0.0f;
    float target_altitude = 0.0f;

    if (!sequencer.getTargetValues(target_roll, target_pitch, target_yaw, target_altitude)) {
        printf("[LevelFlightState::onUpdate] Failed to get target values\n");
        return ProcessStatus::FAILURE;
    }

    // 初期値オフセットを適用（絶対的な目標値に変換）
    target_yaw += context.initial_yaw_offset;
    target_altitude += context.initial_altitude_offset;

    // CascadePIDController を初期化（最初の一度だけ）
    static CascadePIDController cascade_pid;
    static bool cascade_pid_initialized = false;
    
    if (!cascade_pid_initialized) {
        float dt = context.loop_time_us / 1e6f;  // マイクロ秒から秒に変換
        if (!cascade_pid.initialize(dt)) {
            printf("[LevelFlightState::onUpdate] Failed to initialize CascadePIDController\n");
            return ProcessStatus::FAILURE;
        }
        cascade_pid_initialized = true;
    }
    
    // Pitch 軸
    float target_pitch_rate = cascade_pid.computeOuterLoop(0, target_pitch, context.attitude_state.angle.pitch());
    float pid_result_pitch = cascade_pid.computeInnerLoop(0, target_pitch_rate, context.attitude_state.rate.pitch());
    
    // Roll 軸
    float target_roll_rate = cascade_pid.computeOuterLoop(1, target_roll, context.attitude_state.angle.roll());
    float pid_result_roll = cascade_pid.computeInnerLoop(1, target_roll_rate, context.attitude_state.rate.roll());
    
    // Yaw 軸
    float target_yaw_rate = cascade_pid.computeOuterLoop(2, target_yaw, context.attitude_state.angle.yaw());
    float pid_result_yaw = cascade_pid.computeInnerLoop(2, target_yaw_rate, context.attitude_state.rate.yaw());
    
    float pid_result[3] = {pid_result_pitch, pid_result_roll, pid_result_yaw};

    printf("pitch: %f, roll: %f, yaw: %f\n", pid_result[0], pid_result[1], pid_result[2]);

    // TODO: pid_result[0~2]を制御出力に変換してモーター・サーボに指令を送る

    return ProcessStatus::SUCCESS;
}


StateID LevelFlightState::evaluateNextState(StateContext& context) {

    // 安全スティックの値を確認（飛行終了判定）
    if(context.rescaled_sbus_data.safety == SwitchPosition::LOW){

        return StateID::POST_FLIGHT_STATE;
    }

    // 手動飛行の判定
    if(context.rescaled_sbus_data.flight_debug == SwitchPosition::LOW){

        return StateID::MANUAL_FLIGHT_STATE;
    }

    return StateID::LEVEL_FLIGHT_STATE;
}


StateID LevelFlightState::getStateID() const {

    return StateID::LEVEL_FLIGHT_STATE;
}

