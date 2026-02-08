#include "isr_manager.hpp"
#include <stdio.h>

namespace ISRManager {

    // 登録されたSBUSインスタンスへのポインタ
    static nokolat::SBUS* sbus_instance = nullptr;
    static UART_HandleTypeDef* sbus_huart = nullptr;

    // SBUS受信用の大きな循環バッファ（複数フレーム分）
    static constexpr uint8_t BUFFER_SIZE = 75;  // 3フレーム分（25 * 3）
    static constexpr uint8_t INITIAL_RECEIVE_SIZE = 50;  // 初回受信サイズ（2フレーム分）
    static constexpr uint8_t NORMAL_RECEIVE_SIZE = 25;   // 通常受信サイズ（1フレーム分）
    static uint8_t sbus_circular_buffer[BUFFER_SIZE];
    static uint8_t sbus_temp_buffer[INITIAL_RECEIVE_SIZE];  // DMA受信用の一時バッファ（最大サイズ）
    static uint8_t buffer_write_index = 0;

    // フレーム同期状態の管理
    static bool frame_synced = false;
    static bool initial_sync_done = false;  // 初回大量受信完了フラグ
    static uint16_t receive_count = 0;

    // SBUSフレームの定数
    static constexpr uint8_t SBUS_FRAME_SIZE = 25;
    static constexpr uint8_t SBUS_HEADER = 0x0F;
    static constexpr uint8_t SBUS_FOOTER = 0x00;
    static constexpr uint8_t SBUS_FOOTER2 = 0x04;

    // フレーム同期: 循環バッファ内で有効なSBUSフレームを探す
    // 戻り値: 有効なフレームが見つかった場合はtrue、見つからない場合はfalse
    static bool findValidFrame(uint8_t* output_buffer) {

        // 最新データから古いデータへ逆方向に走査
        // これにより、常に最新の有効フレームを確実に取得できる
        for (uint8_t i = 0; i < BUFFER_SIZE - SBUS_FRAME_SIZE + 1; i++) {

            // buffer_write_indexの直前（最新データ）から逆方向に探索
            // +BUFFER_SIZE*2 は負の値を避けるため（modulo演算用）
            uint8_t idx = (buffer_write_index - SBUS_FRAME_SIZE - i + BUFFER_SIZE * 2) % BUFFER_SIZE;
            uint8_t header = sbus_circular_buffer[idx];
            uint8_t footer_idx = (idx + SBUS_FRAME_SIZE - 1) % BUFFER_SIZE;
            uint8_t footer = sbus_circular_buffer[footer_idx];

            // ヘッダーとフッターのチェック
            if (header == SBUS_HEADER &&
                (footer == SBUS_FOOTER || (footer & 0x0F) == SBUS_FOOTER2)) {

                // 有効なフレームが見つかった - 出力バッファにコピー
                for (uint8_t j = 0; j < SBUS_FRAME_SIZE; j++) {
                    output_buffer[j] = sbus_circular_buffer[(idx + j) % BUFFER_SIZE];
                }

                // 初回同期時のみログ出力
                if (!frame_synced) {
                    printf("[ISRManager] SBUS frame sync established after %d attempts (offset from latest: %d)\n",
                           receive_count, i);
                    frame_synced = true;
                }
                return true;
            }
        }

        return false;
    }

    void registerSBUS(nokolat::SBUS* sbus, UART_HandleTypeDef* huart) {

        sbus_instance = sbus;
        sbus_huart = huart;

        // 初回の受信を開始
        if (sbus_instance && sbus_huart) {

            // バッファをクリア
            for (uint8_t i = 0; i < BUFFER_SIZE; i++) {
                sbus_circular_buffer[i] = 0;
            }
            for (uint8_t i = 0; i < INITIAL_RECEIVE_SIZE; i++) {
                sbus_temp_buffer[i] = 0;
            }
            buffer_write_index = 0;
            frame_synced = false;
            initial_sync_done = false;
            receive_count = 0;

            // 初回は大きなバッファ（50バイト = 2フレーム分）で受信
            // これにより、ほぼ確実に少なくとも1つの完全なフレームを取得
            HAL_StatusTypeDef status = HAL_UART_Receive_DMA(sbus_huart, sbus_temp_buffer, INITIAL_RECEIVE_SIZE);
            printf("[ISRManager] SBUS DMA start (Frame sync mode, initial: %d bytes): %s\n",
                   INITIAL_RECEIVE_SIZE, (status == HAL_OK) ? "OK" : "FAILED");
        }
    }

    void handleUartRxCplt(UART_HandleTypeDef* huart) {

        // 登録されたUARTハンドルと一致する場合のみ処理
        if (sbus_instance && sbus_huart == huart) {

            receive_count++;

            // 受信サイズを判定（初回は50バイト、以降は25バイト）
            uint8_t received_size = initial_sync_done ? NORMAL_RECEIVE_SIZE : INITIAL_RECEIVE_SIZE;

            // 受信したデータを循環バッファに追加
            for (uint8_t i = 0; i < received_size; i++) {
                sbus_circular_buffer[buffer_write_index] = sbus_temp_buffer[i];
                buffer_write_index = (buffer_write_index + 1) % BUFFER_SIZE;
            }

            // 初回受信完了フラグを設定
            if (!initial_sync_done) {
                initial_sync_done = true;
                printf("[ISRManager] Initial sync buffer filled (%d bytes)\n", INITIAL_RECEIVE_SIZE);
            }

            // 有効なフレームを探す
            uint8_t valid_frame[SBUS_FRAME_SIZE];
            if (findValidFrame(valid_frame)) {

                // 有効なフレームが見つかった - SBUSのバッファにコピー
                uint8_t* sbus_buffer = sbus_instance->getReceiveBufferPtr();
                for (uint8_t i = 0; i < SBUS_FRAME_SIZE; i++) {
                    sbus_buffer[i] = valid_frame[i];
                }

                // パース要求フラグを立てる
                sbus_instance->requireParse(true);

            } else {
                // フレームが見つからない場合は静かに次の受信を待つ
                // 100回ごとに進捗を表示（デバッグ用）
                if (receive_count % 100 == 0) {
                    printf("[ISRManager] Searching for valid frame... (attempts: %d)\n", receive_count);
                }
            }

            // 次の受信を開始（初回後は25バイトずつ）
            HAL_UART_Receive_DMA(sbus_huart, sbus_temp_buffer, NORMAL_RECEIVE_SIZE);
        }
    }
}
