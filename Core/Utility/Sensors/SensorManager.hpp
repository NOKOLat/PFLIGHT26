#ifndef SENSOR_MANAGER_HPP
#define SENSOR_MANAGER_HPP

#include "main.h"
#include "../../Lib/STM32_ICM42688P/ICM42688P_HAL_I2C.h"
#include "../../Lib/STM32_BMM350/BMM350_Class.hpp"
#include "../../Lib/STM32_DPS368/DPS368_HAL_I2C.hpp"
#include "../Vector3f.hpp"

class SensorManager {

    public:

        SensorManager(I2C_HandleTypeDef* i2c_handle);

        bool initSensors();

        bool configSensors();

        bool CalibrationSensors();

        bool updateSensors();

        bool getAccelData(Vector3f* accel);
        bool getGyroData(Vector3f* gyro);
        bool getMagData(Vector3f* mag);
        bool getPressData(float* press);
        bool getTempData(float* temp);

    private:

        ICM42688P_HAL_I2C icm42688p;
        BMM350 bmm350;
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
};

#endif // SENSOR_MANAGER_HPP
