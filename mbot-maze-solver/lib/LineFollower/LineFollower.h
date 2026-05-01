#pragma once
#include <Arduino.h>

// LineFollower wraps the line follower sensor and performs a small amount of filtering.
class LineFollower {
public:
    // Check if the front sensor is detecting an obstacle (with debouncing).
    bool frontBlocked();

    // Read the raw analog value from the sensor.
    uint8_t readRaw();

private:
    // Counter for consecutive stable readings to debounce sensor noise.
    int stableHits{ 0 };
    
    // Number of consecutive stable hits required to confirm a blocked state.
    static constexpr int REQUIRED_HITS{ 1 };
};
