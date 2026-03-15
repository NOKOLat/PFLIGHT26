#ifndef SENSOR_MANAGER_HPP
#define SENSOR_MANAGER_HPP

#include "main.h"
#include "../../Lib/STM32_ICM42688P/ICM42688P_HAL_I2C.h"
#include "../../Test/MockBMM350/MockBMM350.hpp"
#include "../../Lib/STM32_DPS368/DPS368_HAL_I2C.hpp"
#include "../../Config/sensor_config.hpp"
#include "../../Config/calibration_config.hpp"
#include "../Vector3f.hpp"
#include "../MovingAverage.hpp"

class SensorManager {

    public:

        SensorManager(I2C_HandleTypeDef* i2c_handle);

        bool initSensors();

        bool configSensors();

        bool CalibrationSensors();
        bool performCalibrationIfNeeded();  // enable_calibration_ に基づいてキャリブレーション実行

        bool updateSensors();

        bool getAccelData(Vector3f* accel);
        bool getGyroData(Vector3f* gyro);
        bool getMagData(Vector3f* mag);
        bool getPressData(float* press);
        bool getTempData(float* temp);

        // IMU オフセット値を取得・設定
        bool getAccelOffsets(int16_t offset[3]) const;
        bool getGyroOffsets(int16_t offset[3]) const;
        bool setAccelOffsets(const int16_t offset[3]);
        bool setGyroOffsets(const int16_t offset[3]);

        // 移動平均アクセサ
        float getAltitudeAverage() const;
        float getYawAverage() const;
        void updateMovingAverages(float altitude, float yaw);
        void resetMovingAverages();

        // キャリブレーション設定
        void setEnableCalibration(bool enable) { enable_calibration_ = enable; }
        bool isCalibrationEnabled() const { return enable_calibration_; }

    private:

        ICM42688P_HAL_I2C icm42688p;
        MockBMM350 bmm350;
        DPS368_HAL_I2C dps368;

        // センサーデータバッファ
        float imu_accel_buffer[3] = {0.0f};
        float imu_gyro_buffer[3] = {0.0f};
        float mag_buffer[3] = {0.0f};
        float press_buffer = 0.0f;
        float temp_buffer = 0.0f;

        // 地磁気センサーキャリブレーション値
        float mag_offset[3] = {0.0f, 0.0f, 0.0f};
        float mag_scale[3] = {1.0f, 1.0f, 1.0f};

        // 移動平均ユーティリティ（高度、ヨー角）
        MovingAverage<float, 20> altitude_average;
        MovingAverage<float, 5> yaw_average;

        // キャリブレーション設定
        bool enable_calibration_ = CalibrationConfig::ENABLE_CALIBRATION;
};

#endif // SENSOR_MANAGER_HPP
