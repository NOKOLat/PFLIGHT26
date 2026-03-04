#include "MockBMM350.hpp"

MockBMM350::MockBMM350(I2C_HandleTypeDef* hi2c, uint8_t i2c_addr7) {
    (void)hi2c;
    (void)i2c_addr7;
}

uint8_t MockBMM350::init() {
    return 0;
}

uint8_t MockBMM350::config(uint8_t odr, uint8_t ave) {
    (void)odr;
    (void)ave;
    return 0;
}

uint8_t MockBMM350::getdata(float data[3]) {
    data[0] = 0.0f;
    data[1] = 0.0f;
    data[2] = 0.0f;
    return 0;
}
