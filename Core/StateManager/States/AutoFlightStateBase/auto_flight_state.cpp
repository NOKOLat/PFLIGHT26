#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"

ProcessStatus AutoFlightState::onUpdate(StateContext& context) {

    // ManeuverSequencer が初期化されているか確認
    if (!context.instances.maneuver_sequencer.has_value()) {
        return ProcessStatus::FAILURE;
    }

    ManeuverSequencer& sequencer = context.instances.maneuver_sequencer.value();

    // 初回のみミッション開始
    if (!mission_started_) {

        if (context.current_mission == nullptr) {
            printf("[AutoFlightState::onUpdate] No mission set\n");
            return ProcessStatus::FAILURE;
        }

        sequencer.startMission(context.current_mission);
        mission_started_ = true;

        printf("[AutoFlightState] Mission started\n");
    }

    // 目標値を取得
    float target_roll = 0.0f;
    float target_pitch = 0.0f;
    float target_yaw = 0.0f;
    float target_altitude = 0.0f;

    if (!sequencer.getTargetValues(target_roll, target_pitch, target_yaw, target_altitude)) {
        printf("[AutoFlightState::onUpdate] Failed to get target values\n");
        return ProcessStatus::FAILURE;
    }

    // アクティブチャンネルを確認
    uint8_t active_channels = sequencer.getActiveChannels();

    // アクティブなチャンネルのみ PID に目標値と現在値を渡して計算
    if (active_channels & ChannelFlags::ROLL) {
        // PID 計算は CascadePIDController で処理
        // context.instances.angle_roll_pid.value().calc(target_roll, context.attitude_state.angle.roll());
        printf("[AutoFlightState] Roll - Target: %.2f, Current: %.2f\n", target_roll, context.attitude_state.angle.roll());
    }

    if (active_channels & ChannelFlags::PITCH) {
        // PID 計算は CascadePIDController で処理
        // context.instances.angle_pitch_pid.value().calc(target_pitch, context.attitude_state.angle.pitch());
        printf("[AutoFlightState] Pitch - Target: %.2f, Current: %.2f\n", target_pitch, context.attitude_state.angle.pitch());
    }

    if (active_channels & ChannelFlags::YAW) {
        // PID 計算は CascadePIDController で処理
        // context.instances.angle_yaw_pid.value().calc(target_yaw, context.attitude_state.angle.yaw());
        printf("[AutoFlightState] Yaw - Target: %.2f, Current: %.2f\n", target_yaw, context.attitude_state.angle.yaw());
    }

    // 高度制御（今後実装）
    if (active_channels & ChannelFlags::ALTITUDE) {
        printf("[AutoFlightState] Target altitude: %.2f m\n", target_altitude);
    }

    return ProcessStatus::SUCCESS;
}


StateID AutoFlightState::evaluateNextState(StateContext& context) {

    // ミッション完了判定
    if (context.instances.maneuver_sequencer.has_value()) {

        if (context.instances.maneuver_sequencer.value().isMissionComplete()) {
            printf("[AutoFlightState] Mission complete, transitioning to AUTO_LANDING_STATE\n");
            return StateID::AUTO_LANDING_STATE;
        }
    }

    // デフォルトは現在の状態を継続
    return StateID::AUTO_FLIGHT_STATE;
}


StateID AutoFlightState::getStateID() const {

    return StateID::AUTO_FLIGHT_STATE;
}
