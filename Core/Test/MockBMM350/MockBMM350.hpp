#ifndef MOCK_BMM350_HPP
#define MOCK_BMM350_HPP

#include "main.h"

/**
 * @brief BMM350のモッククラス
 *
 * BMM350_Classと同一のインターフェースを持ち、すべての処理が成功を返す。
 * センサーデータは常に (0, 0, 0) を返す。
 * テスト・デバッグ用途で使用する。
 */
class MockBMM350 {
public:
    /**
     * @brief コンストラクタ
     * @param hi2c I2C_HandleTypeDef へのポインタ（未使用）
     * @param i2c_addr7 7ビットI2Cアドレス（未使用）
     */
    MockBMM350(I2C_HandleTypeDef* hi2c, uint8_t i2c_addr7);

    /**
     * @brief センサー初期化（常に成功を返す）
     * @return 0（成功）
     */
    uint8_t init();

    /**
     * @brief ODR・アベレージング設定（常に成功を返す）
     * @param odr データレート（未使用）
     * @param ave アベレージング（未使用）
     * @return 0（成功）
     */
    uint8_t config(uint8_t odr, uint8_t ave);

    /**
     * @brief 磁気センサーデータ取得（常に 0, 0, 0 を返す）
     * @param data X, Y, Z データを格納する配列 [uT]
     * @return 0（成功）
     */
    uint8_t getdata(float data[3]);
};

#endif // MOCK_BMM350_HPP
