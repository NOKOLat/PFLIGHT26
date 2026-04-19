#include "context_printer.hpp"
#include <cstdio>

namespace ContextPrinter {

void printServo(const StateContext& context) {
    printf("Servo: ele: %f, rud: %f, ailL: %f, ailR: %f\n",
        context.control_output.servo_pwm.elevator(),
        context.control_output.servo_pwm.rudder(),
        context.control_output.servo_pwm.left_aileron(),
        context.control_output.servo_pwm.right_aileron());
}

void printPidResult(const float pid_result[3]) {
    printf("PID: pitch: %f, roll: %f, yaw: %f\n",
        pid_result[0], pid_result[1], pid_result[2]);
}

void printAttitude(const StateContext& context) {
    printf("Attitude: roll: %.2f, pitch: %.2f, yaw: %.2f\n",
        context.attitude.roll(),
        context.attitude.pitch(),
        context.attitude.yaw());
}

void printSbusRaw(const StateContext& context) {
    printf("SBUS Raw: [0]=%d [1]=%d [2]=%d [3]=%d [5]=%d\n",
        context.rescaled_sbus_data.raw_data[0],
        context.rescaled_sbus_data.raw_data[1],
        context.rescaled_sbus_data.raw_data[2],
        context.rescaled_sbus_data.raw_data[3],
        context.rescaled_sbus_data.raw_data[5]);
}

} // namespace ContextPrinter
