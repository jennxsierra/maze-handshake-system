#pragma once
#include <Arduino.h>

// SonarRange wraps the ultrasonic sensor and returns averaged distance values.
class SonarRange {
public:
    uint16_t centimeters();
};
