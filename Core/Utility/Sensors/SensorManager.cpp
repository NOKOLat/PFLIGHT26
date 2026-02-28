#include "Utility/Sensors/SensorManager.hpp"
#include "../../Config/sensor_i2c_config.hpp"
#include <cstring>

SensorManager::SensorManager(I2C_HandleTypeDef* i2c_handle)
    : icm42688p(i2c_handle, SensorI2CConfig::ICM42688P_ADDR),
      bmm350(i2c_handle, SensorI2CConfig::BMM350_ADDR),
      dps368(i2c_handle, SensorI2CConfig::DPS368_ADDR) {
    // インスタンスはメンバーイニシャライザーで初期化される
}

bool SensorManager::initSensors() {

    bool success = true;

    // 1. IMU (ICM42688P) の初期化
    if (icm42688p.Connection() != 0) {
        printf("ERROR: ICM42688P connection failed\n");
        success = false;
    }

    // 2. 磁気センサー (BMM350) の初期化
    if (bmm350.init() != 0) {
        printf("ERROR: BMM350 initialization failed\n");
        success = false;
    }

    // 3. 気圧センサー (DPS368) の初期化
    if (dps368.init() != 0) {
        printf("ERROR: DPS368 initialization failed\n");
        success = false;
    }

    return success;
}

bool SensorManager::configSensors() {

    // 1. IMU (ICM42688P) の設定
    icm42688p.AccelConfig(
        ICM42688P::ACCEL_Mode::LowNoize,
        ICM42688P::ACCEL_SCALE::SCALE02g,
        ICM42688P::ACCEL_ODR::ODR00500hz,
        ICM42688P::ACCEL_DLPF::ODR40
    );
    icm42688p.GyroConfig(
        ICM42688P::GYRO_MODE::LowNoize,
        ICM42688P::GYRO_SCALE::Dps0500,
        ICM42688P::GYRO_ODR::ODR00500hz,
        ICM42688P::GYRO_DLPF::ODR40
    );

    // 2. 磁気センサー (BMM350) の設定
    bmm350.config(BMM350_DATA_RATE_400HZ, BMM350_NO_AVERAGING);

    // 3. 気圧センサー (DPS368) の設定
    dps368.pressConfig(MEAS_RATE::_128pr_sec, MEAS_SAMPLING::_002_times);
    dps368.tempConfig(MEAS_RATE::_128pr_sec, MEAS_SAMPLING::_002_times);

    return true;
}

bool SensorManager::CalibrationSensors() {

    icm42688p.Calibration(1000); // 1000サンプルでキャリブレーション

    return true;
}

bool SensorManager::updateSensors() {

    bool success = true;

    // 1. IMU (ICM42688P) のデータ更新
    if (icm42688p.GetData(imu_accel_buffer, imu_gyro_buffer) != 0) {

        printf("WARNING: ICM42688P data read failed\n");
        success = false;
    }

    // 2. 磁気センサー (BMM350) のデータ更新
    if (bmm350.getdata(mag_buffer) != 0) {

        printf("WARNING: BMM350 data read failed\n");
        success = false;
    }

    // 3. 気圧センサー (DPS368) のデータ更新
    if (dps368.getData(&press_buffer, &temp_buffer) != 0) {

        printf("WARNING: DPS368 data read failed\n");
        success = false;
    }

    return success;
}

bool SensorManager::getAccelData(Vector3f* accel) {

    if (accel == nullptr) {
        
        return false;
    }

    // バッファから加速度データを取得
    accel->setX(imu_accel_buffer[0]);
    accel->setY(imu_accel_buffer[1]);
    accel->setZ(imu_accel_buffer[2]);

    return true;
}

bool SensorManager::getGyroData(Vector3f* gyro) {

    if (gyro == nullptr) {

        return false;
    }

    // バッファから角速度データを取得
    gyro->setX(imu_gyro_buffer[0]);
    gyro->setY(imu_gyro_buffer[1]);
    gyro->setZ(imu_gyro_buffer[2]);

    return true;
}

bool SensorManager::getMagData(Vector3f* mag) {

    if (mag == nullptr) {
        return false;
    }

    // バッファから磁気センサーデータを取得
    mag->setX(mag_buffer[0]);
    mag->setY(mag_buffer[1]);
    mag->setZ(mag_buffer[2]);

    // キャリブレーション補正を適用
    mag->setX((mag->getX() - mag_offset[0]) * mag_scale[0]);
    mag->setY((mag->getY() - mag_offset[1]) * mag_scale[1]);
    mag->setZ((mag->getZ() - mag_offset[2]) * mag_scale[2]);

    return true;
}

bool SensorManager::getPressData(float* press) {

    if (press == nullptr) {

        return false;
    }

    // バッファから気圧データを取得
    *press = press_buffer;

    return true;
}

bool SensorManager::getTempData(float* temp) {

    if (temp == nullptr) {
        
        return false;
    }

    // バッファから温度データを取得
    *temp = temp_buffer;

    return true;
}
