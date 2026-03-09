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

        // 初期値オフセットを記録（相対的な目標値計算用）
        context.initial_yaw_offset = context.attitude.yaw();
        context.initial_altitude_offset = context.altitude;

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

    // カスケードPID制御の計算
    float pid_result[3] = {0.0f, 0.0f, 0.0f};  // [pitch, roll, yaw]

    target_roll -= 5.0f;

    if (!calculateCascadePID(context, target_pitch, target_roll, target_yaw, pid_result)) {
        printf("[LevelFlightState::onUpdate] Failed to calculate cascade PID\n");
        return ProcessStatus::FAILURE;
    }
    
    // PIDの値をサーボの角度に入力
    context.control_output.servo_pwm.elevator() = pid_result[0] + 1.58; // pitch制御
    context.control_output.servo_pwm.rudder()   = context.rescaled_sbus_data.rudder   * context.unit_conversion.SBUS_TO_SERVO_DEG;
    //context.control_output.servo_pwm.rudder()   = pid_result[2]; // yaw制御
    context.control_output.servo_pwm.aileron()  = pid_result[1] - 0.79; // roll制御

    // エレベーターのリバースを適応
    context.control_output.servo_pwm.elevator() *= -1;


    // SBUSの値(0~100)をそのままスロットルの値(0~100)に入れる（ここは手動操縦）
    context.control_output.motor_pwm.right() = context.rescaled_sbus_data.throttle;
    context.control_output.motor_pwm.left()  = context.rescaled_sbus_data.throttle;


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


bool LevelFlightState::calculateCascadePID(StateContext& context, float target_pitch, float target_roll, float target_yaw, float pid_result[3]) {

    // CascadePIDManagerインスタンスの確認
    if (!context.instances.cascade_pid_manager.has_value()) {
        printf("[LevelFlightState::calculateCascadePID] Cascade PID manager not initialized\n");
        return false;
    }

    // 現在の姿勢状態を取得
    const Euler3f& attitude = context.attitude;

    // CascadePIDManagerで全軸のPIDを計算
    // NOTE: 角速度は SensorFusionManager から直接取得する必要があるが、
    // 暫定的にここでは使用せず、外側ループのみで制御する
    CascadePIDManager& pid_manager = context.instances.cascade_pid_manager.value();
    pid_result[0] = pid_manager.calcPitch(target_pitch, attitude.pitch(), 0.0f);
    pid_result[1] = pid_manager.calcRoll(target_roll, attitude.roll(), 0.0f);
    pid_result[2] = pid_manager.calcYaw(target_yaw, attitude.yaw(), 0.0f);

    return true;
}
