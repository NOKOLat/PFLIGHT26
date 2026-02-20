#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


ProcessStatus CalibrationState::onUpdate(StateContext& context) {

    // imuキャリブレーション
    printf("Start Calibration\n");
    context.instances.sensor_manager->CalibrationSensors();
    printf("End Calibration \n");

    return ProcessStatus::SUCCESS;
}


StateID CalibrationState::evaluateNextState(StateContext& context) {

    // キャリブレーション完了後はPRE_FLIGHT_STATEに遷移
    return StateID::PRE_FLIGHT_STATE;
}


StateID CalibrationState::getStateID() const {

    return StateID::CALIBRATION_STATE;
}
