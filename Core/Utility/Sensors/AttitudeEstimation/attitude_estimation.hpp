#ifndef ATTITUDE_ESTIMATION_HPP
#define ATTITUDE_ESTIMATION_HPP

#include <cstdint>
#include <optional>
#include <cmath>
#include "../../Vector3f.hpp"
#include "../../Euler3f.hpp"
#include "../../MovingAverage.hpp"
#include "../SensorManager.hpp"
#include "../../../Lib/IMU_EKF/attitude_ekf.h"
#include "../../../Lib/Altitude_estimation/altitude.h"
#include "../../../Config/sensor_config.hpp"

// センサーから読み込まれる生データ
struct SensorRawData {
    Vector3f accel;              // 加速度 [m/s^2]
    Vector3f gyro;               // 角速度 [rad/s]
    Vector3f mag;                // 磁気 [uT]
    float barometric_pressure;   // 気圧 [Pa]
    float temperature;           // 温度 [℃]
};

// センサーパイプラインの出力（姿勢推定結果のみ）
struct AttitudeState {
    Euler3f angle;               // ロール・ピッチ・ヨー角 [deg]
    Euler3f rate;                // ロール・ピッチ・ヨー角速度 [deg/s]
    float yaw_avg;               // 直近のヨー角の平均 [deg]
    float altitude;              // 高度 [m]
    float altitude_avg;          // 直近高度の平均 [m]
};

/**
 * @class AttitudeEstimation
 * @brief センサー処理を集約し、姿勢推定結果だけを出力
 *
 * 責務：
 * - SensorManagerからの生データ読み込み
 * - EKF/高度推定の更新
 * - 移動平均の計算
 * - 最新の姿勢推定結果を保持
 */
class AttitudeEstimation {
public:
    AttitudeEstimation();
    ~AttitudeEstimation() = default;

    /**
     * @brief 姿勢推定の初期化（内部でSensorManagerも初期化）
     * @param config センサー設定（I2Cハンドルと有効フラグを含む）
     * @return 初期化成功時true
     */
    bool initialize(const SensorConfig& config);

    /**
     * @brief センサーデータを処理し、姿勢推定結果を計算・更新
     * @param dt サンプリング時間 [秒]
     * @return 処理成功時true
     */
    bool update(float dt);

    /**
     * @brief 最新の姿勢推定結果を取得
     * @return AttitudeState 現在の姿勢推定結果
     */
    const AttitudeState& getAttitudeState() const;

    /**
     * @brief 高度推定のキャリブレーションが完了しているか確認する
     * @return キャリブレーション完了時 true
     */
    bool isAltitudeCalibrated();

private:
    // センサーマネージャー（内部で完全に管理）
    std::optional<SensorManager> sensor_manager_;

    // センサー処理と推定
    std::optional<AttitudeEKF_t> attitude_ekf_;
    std::optional<Altitude> altitude_estimator_;

    // 移動平均フィルター
    MovingAverage<float, 20> altitude_average_;
    MovingAverage<float, 5> yaw_average_;

    // 現在の推定結果
    AttitudeState current_attitude_state_;

    // キャリブレーション設定の保持
    AltitudeCalibConfig altitude_calib_config_;

    /**
     * @brief センサーマネージャーから生データを読み込み
     * @param raw_data 読み込まれたデータを格納
     * @return 読み込み成功時true
     */
    bool readSensorData(SensorRawData& raw_data);

    /**
     * @brief EKFと高度推定を更新
     * @param raw_data センサー生データ
     * @param dt サンプリング時間 [秒]
     */
    void updateEstimators(const SensorRawData& raw_data, float dt);

    /**
     * @brief 推定結果を現在の状態に反映
     */
    void updateAttitudeState();
};

#endif // ATTITUDE_ESTIMATION_HPP
