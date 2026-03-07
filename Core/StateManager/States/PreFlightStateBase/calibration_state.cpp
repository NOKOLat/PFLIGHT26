#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


ProcessStatus CalibrationState::onUpdate(StateContext& context) {

    // 高度推定キャリブレーションは update() 内で自動実行される
    // 初回の IMU キャリブレーション（自動/手動）は initialize() 内で実行済み
    float dt = context.loop_time_us / 1000000.0f;
    context.attitude_estimation.update(dt);

    return ProcessStatus::SUCCESS;
}


StateID CalibrationState::evaluateNextState(StateContext& context) {

    // 高度推定のキャリブレーション完了確認
    if (!context.attitude_estimation.isAltitudeCalibrated()) {
        // キャリブレーション継続中
        return StateID::CALIBRATION_STATE;
    }

    // キャリブレーション完了後はPRE_FLIGHT_STATEに遷移
    printf("End Calibration\n");
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);

    return StateID::PRE_FLIGHT_STATE;
}


StateID CalibrationState::getStateID() const {

    return StateID::CALIBRATION_STATE;
}
