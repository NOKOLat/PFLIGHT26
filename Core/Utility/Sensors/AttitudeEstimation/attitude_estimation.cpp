#include "attitude_estimation.hpp"
#include <cstdio>

AttitudeEstimation::AttitudeEstimation() = default;

bool AttitudeEstimation::initialize(I2C_HandleTypeDef* i2c_handle) {
    // SensorManagerの初期化（内部で保持）
    sensor_manager_.emplace(i2c_handle);

    // EKFの初期化
    attitude_ekf_.emplace();
    AttitudeEKF_Init(&attitude_ekf_.value(), 0.02f);  // 50Hz (dt=0.02s)

    // 高度推定の初期化
    altitude_estimator_.emplace();
    altitude_estimator_->Init();

    printf("[AttitudeEstimation] Initialized\n");
    return true;
}

bool AttitudeEstimation::update(float dt) {
    // センサーマネージャーが初期化されているか確認
    if (!sensor_manager_.has_value()) {
        printf("[AttitudeEstimation::update] SensorManager not initialized\n");
        return false;
    }

    // 1. センサーから生データを読み込み
    SensorRawData raw_data;
    if (!readSensorData(raw_data)) {
        return false;
    }

    // 2. EKFと高度推定を更新
    updateEstimators(raw_data, dt);

    // 3. 推定結果を内部状態に反映
    updateAttitudeState();

    return true;
}

bool AttitudeEstimation::readSensorData(SensorRawData& raw_data) {
    // 内部の SensorManager からデータを読み込み
    if (!sensor_manager_.has_value()) {
        return false;
    }

    SensorManager& sensor_mgr = sensor_manager_.value();

    if (!sensor_mgr.getAccelData(&raw_data.accel)) {
        printf("[AttitudeEstimation::readSensorData] Failed to read accel\n");
        return false;
    }

    if (!sensor_mgr.getGyroData(&raw_data.gyro)) {
        printf("[AttitudeEstimation::readSensorData] Failed to read gyro\n");
        return false;
    }

    if (!sensor_mgr.getMagData(&raw_data.mag)) {
        printf("[AttitudeEstimation::readSensorData] Failed to read mag\n");
        return false;
    }

    if (!sensor_mgr.getPressData(&raw_data.barometric_pressure)) {
        printf("[AttitudeEstimation::readSensorData] Failed to read pressure\n");
        return false;
    }

    if (!sensor_mgr.getTempData(&raw_data.temperature)) {
        printf("[AttitudeEstimation::readSensorData] Failed to read temperature\n");
        return false;
    }

    return true;
}

void AttitudeEstimation::updateEstimators(const SensorRawData& raw_data, float dt) {
    if (!attitude_ekf_.has_value()) {
        return;
    }

    // EKFの入力準備（角速度）
    float gyro_input[3] = {
        raw_data.gyro.x(),
        raw_data.gyro.y(),
        raw_data.gyro.z()
    };

    // EKFの測定値準備（加速度を正規化）
    float accel_magnitude = std::sqrt(
        raw_data.accel.x() * raw_data.accel.x() +
        raw_data.accel.y() * raw_data.accel.y() +
        raw_data.accel.z() * raw_data.accel.z()
    );

    float accel_normalized[3] = {0.0f, 0.0f, 0.0f};
    if (accel_magnitude > 0.1f) {  // ゼロ除算回避
        accel_normalized[0] = raw_data.accel.x() / accel_magnitude;
        accel_normalized[1] = raw_data.accel.y() / accel_magnitude;
        accel_normalized[2] = raw_data.accel.z() / accel_magnitude;
    }

    // EKF更新
    AttitudeEKF_Update(&attitude_ekf_.value(), gyro_input, accel_normalized);

    // 高度推定の更新
    if (altitude_estimator_.has_value()) {
        float angle[3] = {
            attitude_ekf_.value().roll,
            attitude_ekf_.value().pitch,
            attitude_ekf_.value().yaw
        };
        float accel_array[3] = {
            raw_data.accel.x(),
            raw_data.accel.y(),
            raw_data.accel.z()
        };

        altitude_estimator_->Update(
            raw_data.barometric_pressure,
            accel_array,
            angle,
            dt
        );
    }
}

void AttitudeEstimation::updateAttitudeState() {
    if (!attitude_ekf_.has_value() || !altitude_estimator_.has_value()) {
        return;
    }

    // EKFから角度を取得（ラジアンからdegreeに変換）
    const float RAD_TO_DEG = 180.0f / 3.14159265358979323846f;

    current_attitude_state_.angle.roll() = attitude_ekf_.value().roll * RAD_TO_DEG;
    current_attitude_state_.angle.pitch() = attitude_ekf_.value().pitch * RAD_TO_DEG;
    current_attitude_state_.angle.yaw() = attitude_ekf_.value().yaw * RAD_TO_DEG;

    // 角速度（EKFから直接取得できない場合は0で初期化）
    current_attitude_state_.rate.roll() = 0.0f;
    current_attitude_state_.rate.pitch() = 0.0f;
    current_attitude_state_.rate.yaw() = 0.0f;

    // ヨー角の移動平均
    current_attitude_state_.yaw_avg = yaw_average_.update(current_attitude_state_.angle.yaw());

    // 高度推定から高度を取得
    float altitude_data[3] = {0.0f};
    altitude_estimator_->GetData(altitude_data);

    current_attitude_state_.altitude = altitude_data[0];  // [0]が高度
    current_attitude_state_.altitude_avg = altitude_average_.update(current_attitude_state_.altitude);
}

const AttitudeState& AttitudeEstimation::getAttitudeState() const {
    return current_attitude_state_;
}

Altitude* AttitudeEstimation::getAltitudeEstimator() {
    if (altitude_estimator_.has_value()) {
        return &altitude_estimator_.value();
    }
    return nullptr;
}

AttitudeEKF_t* AttitudeEstimation::getAttitudeEKF() {
    if (attitude_ekf_.has_value()) {
        return &attitude_ekf_.value();
    }
    return nullptr;
}

SensorManager* AttitudeEstimation::getSensorManager() {
    if (sensor_manager_.has_value()) {
        return &sensor_manager_.value();
    }
    return nullptr;
}
