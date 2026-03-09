#ifndef SENSOR_FUSION_MANAGER_HPP
#define SENSOR_FUSION_MANAGER_HPP

#include <cstdint>
#include "SensorManager.hpp"
#include "attitude_ekf_adapter.hpp"
#include "../../Lib/Altitude_estimation/altitude.h"
#include "../../Utility/Vector3f.hpp"
#include "../../Utility/Euler3f.hpp"

// ===== SensorFusionManager =====
// センサーマネージャー、姿勢推定、高度推定を統合するモジュール
// SensorManager から取得したセンサーデータを融合して、
// 推定された姿勢情報と高度情報を提供する
class SensorFusionManager {

    public:

        // コンストラクタ（SensorManager のポインタを受け取る）
        SensorFusionManager(SensorManager* sensor_manager);

        // デストラクタ
        virtual ~SensorFusionManager() = default;

        // 初期化処理
        // @param dt: サンプリング時間 [秒]
        // @return: 初期化成功時は true
        bool init(float dt);

        // センサー融合を実行
        // SensorManager から最新のセンサーデータを取得し、
        // 姿勢推定・高度推定を更新する
        bool update();

        // ===== 推定値の取得 =====

        // 推定された姿勢（Euler角）を取得
        // @return: roll, pitch, yaw [deg]
        Euler3f getAttitude() const;

        // ロール角を取得 [deg]
        float getRoll() const;

        // ピッチ角を取得 [deg]
        float getPitch() const;

        // ヨー角を取得 [deg]
        float getYaw() const;

        // 推定された高度を取得 [m]
        float getAltitude() const;

        // 推定された高度速度（クライム率）を取得 [m/s]
        float getClimbRate() const;

        // ===== キャリブレーション =====

        // 高度推定のキャリブレーション回数を設定
        void setAltitudeCalibMax(uint32_t calib_max);

        // 高度推定のキャリブレーション完了確認
        bool isAltitudeCalibrated() const;

        // 高度推定のキャリブレーション実行
        // @param pressure_Pa: 気圧センサーの値 [Pa]
        // @param observed_accel: 観測された加速度（垂直成分）[m/s^2]
        void calibrateAltitude(float pressure_Pa, float observed_accel);

        // ===== アクセサー =====

        // 内部のセンサーマネージャーを取得（キャリブレーション等で使用）
        SensorManager* getSensorManager() const;

    private:

        // センサーマネージャー（外部から提供される）
        SensorManager* sensor_manager_;

        // 姿勢推定（EKFベース）- IMU_EKFアダプタ
        AttitudeEkfAdapter attitude_ekf_adapter_;

        // 高度推定（カルマンフィルタベース）
        Altitude altitude_estimator_;

        // 最後に取得したセンサーデータ
        float last_altitude_;
        float last_climb_rate_;

        // 初期化フラグ
        bool is_initialized_ = false;
};

#endif // SENSOR_FUSION_MANAGER_HPP
