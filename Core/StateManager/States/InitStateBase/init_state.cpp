#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"


void InitState::enter(StateContext& context) {


}


StateResult InitState::update(StateContext& context) {

    //1-1. imuの初期化・通信チェック
    if(!context.instances.imu_sensor.has_value()){

        printf("Error: IMU sensor instance is not initialized.\n");
        return {false, false, StateID::INIT_STATE};
    }

    if(!context.instances.imu_sensor->Connection()){

        printf("Error: IMU sensor connection failed.\n");
        return {false, false, StateID::INIT_STATE};
    }

    // 1-2. imuの設定
    context.instances.imu_sensor->AccelConfig(ICM42688P::ACCEL_Mode::LowNoize, ICM42688P::ACCEL_SCALE::SCALE02g, ICM42688P::ACCEL_ODR::ODR00500hz, ICM42688P::ACCEL_DLPF::ODR40);
    context.instances.imu_sensor->GyroConfig(ICM42688P::GYRO_MODE::LowNoize, ICM42688P::GYRO_SCALE::Dps0250, ICM42688P::GYRO_ODR::ODR00500hz, ICM42688P::GYRO_DLPF::ODR40);


    // 1-3. magの初期化・通信チェック
    if(!context.instances.mag_sensor.has_value()){

    	printf("Error: Mag sensor instance is not initialized.\n");
    	return {false, false, StateID::INIT_STATE};
    }

    if(context.instances.mag_sensor->init()){

    	printf("Error: IMU sensor connection failed.\n");
    	return {false, false, StateID::INIT_STATE};
    }

    // 1-4. magの設定
    context.instances.mag_sensor->config(BMM350_DATA_RATE_400HZ, BMM350_NO_AVERAGING);

    // 1-5. baroの初期化・通信チェック
    if(!context.instances.baro_sensor.has_value()){

    	printf("Error: Baro sensor instance is not initialized.\n");
    	return {false, false, StateID::INIT_STATE};
    }

    if(!context.instances.baro_sensor->init()){

    	printf("Error: Baro sensor connection failed.\n");
    	return {false, false, StateID::INIT_STATE};
    }

    // 1-6. baroの設定
    context.instances.baro_sensor->pressConfig(MEAS_RATE::_128pr_sec, MEAS_SAMPLING::_001_times);
    context.instances.baro_sensor->tempConfig(MEAS_RATE::_128pr_sec, MEAS_SAMPLING::_001_times);

    // 1-7 Motorの初期化チェック(成功 == 1)
    if((context.instances.left_motor.has_value() & context.instances.right_motor.has_value()) != 1){

        printf("Error: Motor instance is not initialized.\n");
        return {false, false, StateID::INIT_STATE};
    }

    if((context.instances.left_motor->isInitialized() & context.instances.right_motor->isInitialized()) != 1){

    	printf("Error: Motor instance is not initialized.\n");
    	return {false, false, StateID::INIT_STATE};
    }

    StateResult result;
    result.success = true;
    result.should_transition = true;
    result.next_state_id = StateID::CALIBRATION_STATE;

    return result;
}


void InitState::exit(StateContext& context) {

    // クリーンアップ処理
}


StateID InitState::getStateID() const {

    return StateID::INIT_STATE;
}


StateBaseID InitState::getStateBaseID() const {

    return StateBaseID::INIT_STATE_BASE;
}
