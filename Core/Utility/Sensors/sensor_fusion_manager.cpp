#include "sensor_fusion_manager.hpp"
#include <cmath>

// コンストラクタ
SensorFusionManager::SensorFusionManager(SensorManager* sensor_manager)
    : sensor_manager_(sensor_manager),
      last_altitude_(0.0f),
      last_climb_rate_(0.0f) {

    // メンバ変数は初期化リストで初期化される
}

// 初期化処理
bool SensorFusionManager::init(float dt) {

    // 1. SensorManager が有効か確認
    if (sensor_manager_ == nullptr) {

        printf("Error: SensorManager is nullptr\n");
        return false;
    }

    // 2. 姿勢推定器を初期化
    if (!attitude_ekf_adapter_.init(dt)) {

        printf("Error: AttitudeEkfAdapter init failed\n");
        return false;
    }

    // 3. 高度推定器を初期化
    altitude_estimator_.Init();

    // 4. 初期化完了
    is_initialized_ = true;

    printf("SensorFusionManager initialized successfully\n");
    return true;
}

// センサー融合を実行
bool SensorFusionManager::update() {

    // 1. 初期化確認
    if (!is_initialized_ || sensor_manager_ == nullptr) {

        printf("Warning: SensorFusionManager not initialized\n");
        return false;
    }

    // 2. SensorManager からセンサーハードウェアの最新データを更新
    if (!sensor_manager_->updateSensors()) {

        printf("Warning: Failed to update sensors\n");
        return false;
    }

    // 3. バッファからセンサーデータを取得
    Vector3f accel, gyro, mag;
    float pressure, temp;

    if (!sensor_manager_->getAccelData(&accel) ||
        !sensor_manager_->getGyroData(&gyro) ||
        !sensor_manager_->getMagData(&mag) ||
        !sensor_manager_->getPressData(&pressure) ||
        !sensor_manager_->getTempData(&temp)) {

        printf("Warning: Failed to get sensor data\n");
        return false;
    }

    // 5. 姿勢推定を実行（加速度 + ジャイロ）
    // 加速度をノーマライズする（重力加速度で割る）
    float accel_magnitude = sqrtf(accel.x() * accel.x() +
                                   accel.y() * accel.y() +
                                   accel.z() * accel.z());

    if (accel_magnitude < 0.001f) {

        printf("Warning: Acceleration magnitude too small\n");
        return false;
    }

    // 正規化された加速度を計算
    Vector3f normalized_accel;
    normalized_accel.x() = accel.x() / accel_magnitude;
    normalized_accel.y() = accel.y() / accel_magnitude;
    normalized_accel.z() = accel.z() / accel_magnitude;

    // ジャイロデータを deg/s から rad/s に変換（ICM42688Pは deg/s で返す）
    constexpr float DEG_TO_RAD = 3.14159265358979323846f / 180.0f;
    Vector3f gyro_rad;
    gyro_rad.x() = gyro.x() * DEG_TO_RAD;
    gyro_rad.y() = gyro.y() * DEG_TO_RAD;
    gyro_rad.z() = gyro.z() * DEG_TO_RAD;

    // Vector3f を float[] に詰め替えてアダプタに渡す
    float accel_f[3] = {normalized_accel.x(), normalized_accel.y(), normalized_accel.z()};
    float gyro_f[3]  = {gyro_rad.x(),         gyro_rad.y(),         gyro_rad.z()};

    if (!attitude_ekf_adapter_.update(accel_f, gyro_f)) {

        printf("Warning: AttitudeEkfAdapter update failed\n");
        return false;
    }

    // 6. 高度推定を実行（気圧 + 加速度の垂直成分 + 姿勢）
    // 姿勢情報をラジアンで取得（二重変換が解消される）
    float attitude_array[3];
    attitude_ekf_adapter_.getAttitudeRad(attitude_array);  // [rad]

    float accel_array[3] = {accel.x(), accel.y(), accel.z()};

    // 高度推定器を更新（サンプリング時間は固定値を使用）
    // NOTE: 実際のサンプリング時間を使用する場合は、dt を渡すように修正が必要
    constexpr float DT = 0.01f;  // 10ms = 100Hz
    altitude_estimator_.Update(pressure, accel_array, attitude_array, DT);

    // 7. 推定値を取得して保存
    float altitude_data[3];
    altitude_estimator_.GetData(altitude_data);
    last_altitude_ = altitude_data[0];      // 高度
    last_climb_rate_ = altitude_data[1];    // 上昇速度

    return true;
}

// 推定された姿勢を取得 [deg]
Euler3f SensorFusionManager::getAttitude() const {

    constexpr float RAD_TO_DEG = 180.0f / 3.14159265358979323846f;
    Euler3f result;
    result.roll()  = attitude_ekf_adapter_.getRollRad()  * RAD_TO_DEG;
    result.pitch() = attitude_ekf_adapter_.getPitchRad() * RAD_TO_DEG;
    result.yaw()   = attitude_ekf_adapter_.getYawRad()   * RAD_TO_DEG;
    return result;
}

// ロール角を取得 [deg]
float SensorFusionManager::getRoll() const {

    constexpr float RAD_TO_DEG = 180.0f / 3.14159265358979323846f;
    return attitude_ekf_adapter_.getRollRad() * RAD_TO_DEG;
}

// ピッチ角を取得 [deg]
float SensorFusionManager::getPitch() const {

    constexpr float RAD_TO_DEG = 180.0f / 3.14159265358979323846f;
    return attitude_ekf_adapter_.getPitchRad() * RAD_TO_DEG;
}

// ヨー角を取得 [deg]
float SensorFusionManager::getYaw() const {

    constexpr float RAD_TO_DEG = 180.0f / 3.14159265358979323846f;
    return attitude_ekf_adapter_.getYawRad() * RAD_TO_DEG;
}

// 推定された高度を取得
float SensorFusionManager::getAltitude() const {

    return last_altitude_;
}

// 推定された高度速度を取得
float SensorFusionManager::getClimbRate() const {

    return last_climb_rate_;
}

// 高度推定のキャリブレーション回数を設定
void SensorFusionManager::setAltitudeCalibMax(uint32_t calib_max) {

    altitude_estimator_.SetCalibMax(calib_max);
}

// 高度推定のキャリブレーション完了確認
bool SensorFusionManager::isAltitudeCalibrated() const {

    return altitude_estimator_.isCalibrated();
}

// 高度推定のキャリブレーション実行
void SensorFusionManager::calibrateAltitude(float pressure_Pa, float observed_accel) {

    altitude_estimator_.Calibration(pressure_Pa, observed_accel);
}

// 内部のセンサーマネージャーを取得
SensorManager* SensorFusionManager::getSensorManager() const {

    return sensor_manager_;
}
