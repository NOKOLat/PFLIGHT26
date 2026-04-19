#pragma once

#include "../../StateManager/StateContext/context.hpp"

// ContextPrinter: StateContext の内容を printf で出力するユーティリティ関数群
namespace ContextPrinter {

    // サーボ角度を表示 [deg]
    // 出力例: "Servo: ele: 0.00, rud: 0.00, ailL: 0.00, ailR: 0.00"
    void printServo(const StateContext& context);

    // PID計算結果を表示 [deg]
    // 出力例: "PID: pitch: 0.00, roll: 0.00, yaw: 0.00"
    void printPidResult(const float pid_result[3]);

    // 姿勢角を表示 [deg]
    // 出力例: "Attitude: roll: 0.00, pitch: 0.00, yaw: 0.00"
    void printAttitude(const StateContext& context);

    // SBUS生データを表示
    // 出力例: "SBUS Raw: [0]=1024 [1]=1024 [2]=352 [3]=1024 [5]=1024"
    void printSbusRaw(const StateContext& context);

} // namespace ContextPrinter
