#pragma once

#include "../../Lib/IMU_EKF/attitude_ekf.h"

// ===== AttitudeEkfAdapter =====
// IMU_EKFのCコア(attitude_ekf.h)をプロジェクト層でラップ
// float[]でのみ通信し、プロジェクト型(Vector3f/Euler3f)への依存を避ける
// これによりIMU_EKFサブモジュールの独立性を保証する
class AttitudeEkfAdapter {

    public:

        AttitudeEkfAdapter();

        // 初期化処理（サンプリング時間を受け取る）
        bool init(float dt);

        // EKFをリセット
        bool reset();

        // センサーデータで更新（float[]のみ使用）
        // @param accel: 加速度データ [m/s^2]（正規化済み）
        // @param gyro:  角速度データ [rad/s]
        bool update(const float accel[3], const float gyro[3]);

        // 推定された Euler 角をラジアンで取得
        // @param angles: 出力用配列 [rad]
        void getAttitudeRad(float angles[3]) const;

        // ロール角を取得 [rad]
        float getRollRad() const;

        // ピッチ角を取得 [rad]
        float getPitchRad() const;

        // ヨー角を取得 [rad]
        float getYawRad() const;

    private:

        // IMU_EKFのC実装
        AttitudeEKF_t ekf_sys_;
};
