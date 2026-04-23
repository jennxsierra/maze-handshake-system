#pragma once
#include <Arduino.h>

// Thin C-style HAL so logic never touches device classes directly.
namespace Hardware {
    void initBle(long baud);
    bool bleAvailable();
    char bleRead();
    void bleWrite(char c);

    void setRgb(uint8_t r, uint8_t g, uint8_t b);

    void setMotors(int left, int right);

    uint16_t readUltrasonicCm();
    uint8_t readLineSensors();
}
