#ifndef ISR_MANAGER_HPP
#define ISR_MANAGER_HPP

#include "stm32f7xx_hal.h"
#include "SBUS/sbus.h"

// 割り込み処理とアプリケーションロジックを仲介する静的クラス
namespace ISRManager {

    // SBUSインスタンスを登録する
    // sbus: データを流し込む対象のSBUSインスタンス
    // huart: SBUSが接続されているUARTハンドル
    void registerSBUS(nokolat::SBUS* sbus, UART_HandleTypeDef* huart);

    // UART受信完了割り込みハンドラ
    // HAL_UART_RxCpltCallback から呼び出されることを想定
    void handleUartRxCplt(UART_HandleTypeDef* huart);
}

#endif // ISR_MANAGER_HPP
