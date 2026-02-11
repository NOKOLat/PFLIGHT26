#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


void CalibrationState::enter(StateContext& context) {

    // キャリブレーション処理の初期化
}


StateResult CalibrationState::update(StateContext& context) {

	// imuキャリブレーション
	printf("Start IMU Calibration\n");

    if(context.instances.imu_sensor->Calibration(1000) != 0){

        printf("IMU Calibration Failed\n");
        return {false, false, StateID::CALIBRATION_STATE};
    }

    printf("End Imu Calibration \n");

    StateResult result;
    result.success = true;
    result.should_transition = true;
    result.next_state_id = StateID::PRE_FLIGHT_STATE;

    return result;
}


void CalibrationState::exit(StateContext& context) {

    // クリーンアップ処理
}


StateID CalibrationState::getStateID() const {

    return StateID::CALIBRATION_STATE;
}


StateBaseID CalibrationState::getStateBaseID() const {

    return StateBaseID::PRE_FLIGHT_STATE_BASE;
}
