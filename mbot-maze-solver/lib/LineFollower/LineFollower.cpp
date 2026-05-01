#include "LineFollower.h"

// Hardware-layer function from hal_hardware.cpp
uint8_t hal_line_read();

bool LineFollower::frontBlocked() {
    uint8_t sample = readRaw();

    // Any reading other than 0x00 (black) is treated as "white"
    if (sample != 0x00) {
        ++stableHits;
    } else {
        stableHits = 0;
    }

    return stableHits >= REQUIRED_HITS;
}

// Read the raw analog value from the sensor.
uint8_t LineFollower::readRaw() {
    return hal_line_read();
}
