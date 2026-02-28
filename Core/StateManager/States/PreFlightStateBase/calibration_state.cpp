#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"
#include "Altitude_estimation/altitude.h"


ProcessStatus CalibrationState::onUpdate(StateContext& context) {

    // imuキャリブレーション（初回のみ）
    if (!calibration_started_) {
        printf("Start Calibration\n");
        context.instances.sensor_manager->CalibrationSensors();
        printf("End Calibration \n");
        // 高度推定キャリブレーション回数を設定
        if (context.instances.altitude_estimator.has_value()) {
            context.instances.altitude_estimator->SetCalibMax(1000);
        }
        calibration_started_ = true;
    }

    // センサーデータの取得
    context.instances.sensor_manager->updateSensors();
    context.instances.sensor_manager->getAccelData(&context.sensor_data.accel);
    context.instances.sensor_manager->getPressData(&context.sensor_data.barometric_pressure);

    // 高度推定のキャリブレーション
    if (context.instances.altitude_estimator.has_value()) {
        context.instances.altitude_estimator->Calibration(
            context.sensor_data.barometric_pressure,
            context.sensor_data.accel[Axis::Z]
        );
    }

    return ProcessStatus::SUCCESS;
}


StateID CalibrationState::evaluateNextState(StateContext& context) {

    // 高度推定のキャリブレーション完了確認
    if (context.instances.altitude_estimator.has_value()) {
        if (!context.instances.altitude_estimator->isCalibrated()) {
            // キャリブレーション継続中
            return StateID::CALIBRATION_STATE;
        }
    }

    // キャリブレーション完了後はPRE_FLIGHT_STATEに遷移
    return StateID::PRE_FLIGHT_STATE;
}


StateID CalibrationState::getStateID() const {

    return StateID::CALIBRATION_STATE;
}
