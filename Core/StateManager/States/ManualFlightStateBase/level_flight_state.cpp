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
