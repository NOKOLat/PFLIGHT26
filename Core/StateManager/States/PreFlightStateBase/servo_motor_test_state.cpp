#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


ProcessStatus ServoMotorTestState::onUpdate(StateContext& context) {

    // テストパラメータ
    const uint32_t LOOP_PER_STEP = 100;           // 各ステップあたりのループ数（約0.7秒）
    const uint32_t TOTAL_STEPS_PER_SERVO = 3;     // 各サーボのステップ数（最小、最大、中立）
    const uint32_t TOTAL_SERVOS = 4;              // サーボ総数（エレベーター、ラダー、右エルロン、左エルロン）

    // モーターテストパラメータ (TIM3 CH1: 右モーター)
    const uint32_t SERVO_TEST_TOTAL_LOOPS = TOTAL_SERVOS * TOTAL_STEPS_PER_SERVO * LOOP_PER_STEP;  // 1200ループ
    const uint32_t MOTOR_TEST_DURATION    = 715;  // 約5秒（@7ms/loop）
    const uint32_t MOTOR_TEST_END_LOOP    = SERVO_TEST_TOTAL_LOOPS + MOTOR_TEST_DURATION;

    // ===== フェーズ1: サーボテスト =====
    if (loop_count < (int)SERVO_TEST_TOTAL_LOOPS) {

        // 現在のステップを計算
        uint32_t total_step  = loop_count / LOOP_PER_STEP;
        uint32_t servo_index = total_step / TOTAL_STEPS_PER_SERVO;
        uint32_t step        = total_step % TOTAL_STEPS_PER_SERVO;

        // 各ステップの角度を決定
        float angle_values[4] = {0.0f, 0.0f, 0.0f, 0.0f};

        if (step == 0) {

            // ステップ0: 最小角度（-90度）
            angle_values[servo_index] = -90.0f;
        }
        else if (step == 1) {

            // ステップ1: 最大角度（+90度）
            angle_values[servo_index] = 90.0f;
        }
        else if (step == 2) {

            // ステップ2: 中立位置（0度）
            angle_values[servo_index] = 0.0f;
        }

        // デバッグ情報を表示（各ステップ開始時のみ）
        static uint32_t last_total_step = 0;

        if (total_step != last_total_step) {

            // サーボマッピング (htim1のCH1～CH4):
            // Index 0: Elevator (CH1)
            // Index 1: Rudder (CH2)
            // Index 2: Aileron Right(CH3)
            // Index 3: Aileron Left(CH4)
            const char* servo_names[] = {"Elevator(CH1)", "Rudder(CH2)", "AileronR(CH3)", "AileronL(CH4)"};
            const char* step_names[]  = {"Min(-90°)", "Max(+90°)", "Neutral(0°)"};
            printf("[ServoTest] Servo:%s Step:%s (Loop:%u)\n", servo_names[servo_index], step_names[step], loop_count);
            last_total_step = total_step;
        }

        // サーボを独立制御（テスト用）
        if (context.instances.pwm_controller) {

            context.instances.pwm_controller->setServoAngle_Independent(angle_values);
        }

        loop_count++;
        return ProcessStatus::SUCCESS;
    }

    // ===== フェーズ2: モーターテスト (TIM3 CH1: 右モーター) =====

    // サーボテスト完了時に一度だけサーボを中立位置に戻してモーターテスト開始を通知
    if (loop_count == (int)SERVO_TEST_TOTAL_LOOPS) {

        float neutral_angles[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        if (context.instances.pwm_controller) {

            context.instances.pwm_controller->setServoAngle(neutral_angles);
        }
        printf("[MotorTest] Start: TIM3 CH4 (Right Motor) 20%% for 5s\n");
    }

    if (loop_count < (int)MOTOR_TEST_END_LOOP) {

        float motor_speed[2] = {20.0f, 20.0f};
        if (context.instances.pwm_controller) {

            context.instances.pwm_controller->setMotorSpeed(motor_speed);
        }

        loop_count++;
        return ProcessStatus::SUCCESS;
    }

    // ===== フェーズ3: モーターテスト完了 =====

    // モーターを0%に戻す
    if (loop_count == (int)MOTOR_TEST_END_LOOP) {

        printf("[MotorTest] Complete: TIM3 CH1 stopped\n");
    }

    float motor_stop[2] = {0.0f, 0.0f};
    if (context.instances.pwm_controller) {

        context.instances.pwm_controller->setMotorSpeed(motor_stop);
    }

    loop_count++;
    return ProcessStatus::SUCCESS;
}


StateID ServoMotorTestState::evaluateNextState(StateContext& context) {

    // プリフライトデバッグが0になったらPRE_FLIGHT_STATEに戻る
    if(context.rescaled_sbus_data.preflight_debug == 0){

        return StateID::PRE_FLIGHT_STATE;
    }

    // 遷移せず現在の状態を継続
    return StateID::SERVO_TEST_STATE;
}


StateID ServoMotorTestState::getStateID() const {

    return StateID::SERVO_TEST_STATE;
}
