#pragma once
#include <Arduino.h>

class RangeSensor {
public:
    uint16_t cm();
};

class LineBarrier {
public:
    // returns true when a stable front barrier is detected
    bool blocked();
    uint8_t raw();

private:
    int stable{ 0 };
    static constexpr int THRESH{ 1 };
};
