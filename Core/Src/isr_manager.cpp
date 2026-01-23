#include "isr_manager.hpp"

namespace ISRManager {

    // 登録されたSBUSインスタンスへのポインタ
    static nokolat::SBUS* sbus_instance = nullptr;
    static UART_HandleTypeDef* sbus_huart = nullptr;

    void registerSBUS(nokolat::SBUS* sbus, UART_HandleTypeDef* huart) {
        
        sbus_instance = sbus;
        sbus_huart = huart;

        // 初回の受信を開始
        if (sbus_instance && sbus_huart) {

            HAL_UART_Receive_IT(sbus_huart, sbus_instance->getReceiveBufferPtr(), 25);
        }
    }

    void handleUartRxCplt(UART_HandleTypeDef* huart) {

        // 登録されたUARTハンドルと一致する場合のみ処理
        if (sbus_instance && sbus_huart == huart) {
            
            // パース要求フラグを立てる（メインループでデータアクセス時にパースされる）
            sbus_instance->requireParse(true);

            // 次の受信を開始
            HAL_UART_Receive_IT(sbus_huart, sbus_instance->getReceiveBufferPtr(), 25);
        }
    }
}
