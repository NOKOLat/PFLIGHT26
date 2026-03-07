#include "attitude_estimation.hpp"
#include <cstdio>

AttitudeEstimation::AttitudeEstimation() = default;

bool AttitudeEstimation::initialize(const SensorConfig& config) {

    // SensorManagerの初期化（内部で保持）
    sensor_manager_.emplace(config.i2c_handle, config.i2c_addresses);

    // センサーハードウェア初期化と設定（内部で自動化）
    if (!sensor_manager_->initSensors()) {
        printf("[AttitudeEstimation::initialize] Failed to initialize sensors\n");
        return false;
    }

    if (!sensor_manager_->configSensors()) {
        printf("[AttitudeEstimation::initialize] Failed to configure sensors\n");
        return false;
    }

    // IMUキャリブレーション（自動 or 手動）
    if (config.imu_calib.enable_auto_calibration) {
        printf("[AttitudeEstimation::initialize] Performing IMU auto-calibration...\n");
        if (!sensor_manager_->CalibrationSensors()) {
            printf("[AttitudeEstimation::initialize] IMU calibration failed\n");
            return false;
        }
    } else {
        printf("[AttitudeEstimation::initialize] Applying manual IMU offsets\n");
        sensor_manager_->setAccelOffsets(config.imu_calib.manual_accel_offset);
        sensor_manager_->setGyroOffsets(config.imu_calib.manual_gyro_offset);
    }

    // EKFの初期化
    attitude_ekf_.emplace();
    AttitudeEKF_Init(&attitude_ekf_.value(), 0.02f);  // 50Hz (dt=0.02s)

    // 高度推定の初期化とキャリブレーション設定
    altitude_estimator_.emplace();
    altitude_estimator_->Init();
    altitude_estimator_->SetCalibMax(config.altitude_calib.sample_count);

    // キャリブレーション設定を保持（update()で使用）
    altitude_calib_config_ = config.altitude_calib;

    printf("[AttitudeEstimation::initialize] Initialized successfully\n");
    return true;
}

bool AttitudeEstimation::update(float dt) {
    if (!sensor_manager_.has_value() || !altitude_estimator_.has_value()) {
        printf("[AttitudeEstimation::update] Components not initialized\n");
        return false;
    }

    // 高度推定キャリブレーション中
    if (!altitude_estimator_->isCalibrated()) {
        float pressure = 0.0f;
        if (!sensor_manager_->getPressData(&pressure)) {
            printf("[AttitudeEstimation::update] Failed to get pressure for calibration\n");
            return false;
        }
        altitude_estimator_->Calibration(pressure, altitude_calib_config_.reference_gravity);
        return true;
    }

    // 通常の推定処理
    SensorRawData raw_data;
    if (!readSensorData(raw_data)) {
        return false;
    }

    // EKFと高度推定を更新
    updateEstimators(raw_data, dt);

    // 推定結果を内部状態に反映
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

bool AttitudeEstimation::isAltitudeCalibrated() {
    if (altitude_estimator_.has_value()) {
        return altitude_estimator_->isCalibrated();
    }
    return false;
}
