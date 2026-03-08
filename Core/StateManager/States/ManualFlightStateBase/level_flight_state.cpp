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
    angle_pitch_pid.calc(target_pitch, attitude.angle.pitch());
    float target_pitch_rate = angle_pitch_pid.getData();

    // 内側ループ（角速度制御）: 目標角速度 → 制御出力（2回に1回のみ実行）
    PID& rate_pitch_pid = context.instances.rate_pitch_pid.value();
    if (inner_loop_counter_ == 0) {
        rate_pitch_pid.calc(target_pitch_rate, attitude.rate.pitch());
    }
    pid_result[0] = rate_pitch_pid.getData();

    // ========== Roll軸 ==========
    // 外側ループ（角度制御）: 目標角度 → 目標角速度
    PID& angle_roll_pid = context.instances.angle_roll_pid.value();
    angle_roll_pid.calc(target_roll, attitude.angle.roll());
    float target_roll_rate = angle_roll_pid.getData();

    // 内側ループ（角速度制御）: 目標角速度 → 制御出力（2回に1回のみ実行）
    PID& rate_roll_pid = context.instances.rate_roll_pid.value();
    if (inner_loop_counter_ == 0) {
        rate_roll_pid.calc(target_roll_rate, attitude.rate.roll());
    }
    pid_result[1] = rate_roll_pid.getData();

    // ========== Yaw軸 ==========
    // 外側ループ（角度制御）: 目標角度 → 目標角速度
    PID& angle_yaw_pid = context.instances.angle_yaw_pid.value();
    angle_yaw_pid.calc(target_yaw, attitude.angle.yaw());
    float target_yaw_rate = angle_yaw_pid.getData();

    // 内側ループ（角速度制御）: 目標角速度 → 制御出力（2回に1回のみ実行）
    PID& rate_yaw_pid = context.instances.rate_yaw_pid.value();
    if (inner_loop_counter_ == 0) {
        rate_yaw_pid.calc(target_yaw_rate, attitude.rate.yaw());
    }
    pid_result[2] = rate_yaw_pid.getData();

    // カウンターを更新（0と1を交互に）
    inner_loop_counter_ = (inner_loop_counter_ + 1) % 2;

    return true;
}
