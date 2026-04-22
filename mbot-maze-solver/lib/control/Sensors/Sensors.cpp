#include "Sensors.h"
#include "hardware_hal.h"

uint16_t RangeSensor::cm() {
    uint16_t a = Hardware::readUltrasonicCm();
    uint16_t b = Hardware::readUltrasonicCm();
    uint16_t c = Hardware::readUltrasonicCm();
    return (a + b + c) / 3;
}

uint8_t LineBarrier::raw() {
    return Hardware::readLineSensors();
}

bool LineBarrier::blocked() {
    uint8_t v = raw();
    if (v != 0x00) {
        ++stable;
    } else {
        stable = 0;
    }
    return stable >= THRESH;
}
