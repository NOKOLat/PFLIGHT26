#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"
#include "Altitude_estimation/altitude.h"
#include "../../../Config/calibration_config.hpp"


ProcessStatus CalibrationState::onUpdate(StateContext& context) {

    // imuキャリブレーション（初回のみ）
    if (!calibration_started_) {

        printf("Start Calibration\n");

        // キャリブレーション有効フラグに応じた処理
        if (enable_calibration_) {

            PerformSensorCalibration(context);
        } 
        else {

            ApplyManualCalibrationOffsets(context);
        }

        // 高度推定キャリブレーション回数を設定
        if (context.instances.altitude_estimator.has_value()) {

            context.instances.altitude_estimator->SetCalibMax(10);
        }
        calibration_started_ = true;
    }

    // センサーデータの取得
    context.instances.sensor_manager->updateSensors();
    context.instances.sensor_manager->getAccelData(&context.sensor_data.accel);
    context.instances.sensor_manager->getPressData(&context.sensor_data.barometric_pressure);

    // 高度推定のキャリブレーション
    // キャリブレーションで、加速度のz成分が重力加速度と等しいと仮定
    if (context.instances.altitude_estimator.has_value()) {

        context.instances.altitude_estimator->Calibration(
            context.sensor_data.barometric_pressure,
            9.80665f
        );
    }

    return ProcessStatus::SUCCESS;
}


ProcessStatus CalibrationState::PerformSensorCalibration(StateContext& context) {

    printf("[Calibration] Executing sensor calibration...\n");
    context.instances.sensor_manager->CalibrationSensors();

    // オフセットの値を出力
    int16_t accel_offset[3] = {0};
    int16_t gyro_offset[3] = {0};

    if (context.instances.sensor_manager->getAccelOffsets(accel_offset)) {
        printf("[Calibration] Accel Offsets - X: %d, Y: %d, Z: %d\n",
               accel_offset[0], accel_offset[1], accel_offset[2]);
    }

    if (context.instances.sensor_manager->getGyroOffsets(gyro_offset)) {
        printf("[Calibration] Gyro Offsets - X: %d, Y: %d, Z: %d\n",
               gyro_offset[0], gyro_offset[1], gyro_offset[2]);
    }

    return ProcessStatus::SUCCESS;
}


ProcessStatus CalibrationState::ApplyManualCalibrationOffsets(StateContext& context) {
    printf("[Calibration] Using manual offset values from config\n");

    int16_t manual_accel_offset[3] = {
        CalibrationConfig::MANUAL_ACCEL_OFFSET_X,
        CalibrationConfig::MANUAL_ACCEL_OFFSET_Y,
        CalibrationConfig::MANUAL_ACCEL_OFFSET_Z
    };
    int16_t manual_gyro_offset[3] = {
        CalibrationConfig::MANUAL_GYRO_OFFSET_X,
        CalibrationConfig::MANUAL_GYRO_OFFSET_Y,
        CalibrationConfig::MANUAL_GYRO_OFFSET_Z
    };

    context.instances.sensor_manager->setAccelOffsets(manual_accel_offset);
    context.instances.sensor_manager->setGyroOffsets(manual_gyro_offset);

    printf("[Calibration] Manual Accel Offsets - X: %d, Y: %d, Z: %d\n",
           manual_accel_offset[0], manual_accel_offset[1], manual_accel_offset[2]);
    printf("[Calibration] Manual Gyro Offsets - X: %d, Y: %d, Z: %d\n",
           manual_gyro_offset[0], manual_gyro_offset[1], manual_gyro_offset[2]);

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

    // GPIO設定
    printf("End Calibration \n");

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);

    return StateID::PRE_FLIGHT_STATE;
}


StateID CalibrationState::getStateID() const {

    return StateID::CALIBRATION_STATE;
}
