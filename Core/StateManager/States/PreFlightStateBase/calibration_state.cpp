#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


void CalibrationState::onEnter(StateContext& context) {

    // キャリブレーション処理の初期化
}


StateResult CalibrationState::onUpdate(StateContext& context) {

    // imuキャリブレーション
    printf("Start Calibration\n");
    context.instances.sensor_manager->CalibrationSensors();
    printf("End Calibration \n");

    return {true, false, StateID::CALIBRATION_STATE};
}


void CalibrationState::onExit(StateContext& context) {

    // クリーンアップ処理
}


StateID CalibrationState::evaluateNextState(StateContext& context) {

    // キャリブレーション完了後はPRE_FLIGHT_STATEに遷移
    return StateID::PRE_FLIGHT_STATE;
}


StateID CalibrationState::getStateID() const {

    return StateID::CALIBRATION_STATE;
}
