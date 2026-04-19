#include "../StateHeaders.hpp"
#include "../../StateContext/context.hpp"
#include "../../../Config/calibration_config.hpp"


ProcessStatus CalibrationState::onUpdate(StateContext& context) {

    // 初期化確認
    if (!context.instances.sensor_fusion_manager.has_value()) {

        printf("Error: SensorFusionManager is not initialized\n");
        return ProcessStatus::FAILURE;
    }

    // imuキャリブレーション（初回のみ）
    if (!calibration_started_) {

        printf("Start Calibration\n");

        // SensorManager にアクセス
        SensorManager* sensor_mgr = context.instances.sensor_fusion_manager->getSensorManager();
        if (sensor_mgr == nullptr) {
            printf("[Calibration] Error: Cannot access SensorManager\n");
            return ProcessStatus::FAILURE;
        }

        // キャリブレーション有効フラグに応じた処理（SensorManager が管理）
        if (!sensor_mgr->performCalibrationIfNeeded()) {

            printf("[Calibration] Error: performCalibrationIfNeeded failed\n");
            return ProcessStatus::FAILURE;
        }

        // キャリブレーション実行時は測定値を出力して停止
        if (sensor_mgr->isCalibrationEnabled()) {

            int16_t accel_offset[3] = {0, 0, 0};
            int16_t gyro_offset[3] = {0, 0, 0};
            sensor_mgr->getAccelOffsets(accel_offset);
            sensor_mgr->getGyroOffsets(gyro_offset);

            printf("[Calibration] === Calibration Result ===\n");
            printf("[Calibration] Accel Offsets - X: %d, Y: %d, Z: %d\n",
                   accel_offset[0], accel_offset[1], accel_offset[2]);
            printf("[Calibration] Gyro Offsets  - X: %d, Y: %d, Z: %d\n",
                   gyro_offset[0], gyro_offset[1], gyro_offset[2]);
            printf("[Calibration] -> calibration_config.hpp を上記の値で更新し、ENABLE_CALIBRATION = false にしてください\n");

            while (1) {
                // キャリブレーション測定完了。値を確認後、calibration_config.hpp を更新してください
            }
        }

        // 手動設定値を使用する場合は適用
        if (!sensor_mgr->isCalibrationEnabled()) {

            if (ApplyManualCalibrationOffsets(context) != ProcessStatus::SUCCESS) {

                return ProcessStatus::FAILURE;
            }
        }

        // 高度推定キャリブレーション回数を設定
        context.instances.sensor_fusion_manager->setAltitudeCalibMax(10);

        calibration_started_ = true;
    }

    // センサー融合を更新
    context.instances.sensor_fusion_manager->update();

    // 高度推定のキャリブレーション
    // DPS368 の気圧[Pa]を使用して高度推定キャリブレーションを実施する
    SensorManager* sensor_mgr = context.instances.sensor_fusion_manager->getSensorManager();

    if (sensor_mgr != nullptr) {

    	for(uint16_t i=0; i<1000; i++){

			float pressure_Pa = 0.0f;
			sensor_mgr->getPressData(&pressure_Pa);
			context.instances.sensor_fusion_manager->calibrateAltitude(pressure_Pa, 9.80665f);
    	}
    }

    return ProcessStatus::SUCCESS;
}


ProcessStatus CalibrationState::ApplyManualCalibrationOffsets(StateContext& context) {
    printf("[Calibration] Using manual offset values from config\n");

    // SensorManager にアクセス
    SensorManager* sensor_mgr = context.instances.sensor_fusion_manager->getSensorManager();

    if (sensor_mgr == nullptr) {

        printf("[Calibration] Error: Cannot access SensorManager\n");
        return ProcessStatus::FAILURE;
    }

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

    sensor_mgr->setAccelOffsets(manual_accel_offset);
    sensor_mgr->setGyroOffsets(manual_gyro_offset);

    printf("[Calibration] Manual Accel Offsets - X: %d, Y: %d, Z: %d\n",
           manual_accel_offset[0], manual_accel_offset[1], manual_accel_offset[2]);
    printf("[Calibration] Manual Gyro Offsets - X: %d, Y: %d, Z: %d\n",
           manual_gyro_offset[0], manual_gyro_offset[1], manual_gyro_offset[2]);

    return ProcessStatus::SUCCESS;
}


StateID CalibrationState::evaluateNextState(StateContext& context) {

    // 高度推定のキャリブレーション完了確認
    if (context.instances.sensor_fusion_manager.has_value()) {

        if (!context.instances.sensor_fusion_manager->isAltitudeCalibrated()) {

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
