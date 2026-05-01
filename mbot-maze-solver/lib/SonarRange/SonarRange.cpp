#include "SonarRange.h"

// Hardware-layer function from hal_hardware.cpp
uint16_t hal_sonar_cm();

// Returns the average distance in centimeters from three
// sonar readings to reduce noise.
uint16_t SonarRange::centimeters() {
    uint16_t first = hal_sonar_cm();
    uint16_t second = hal_sonar_cm();
    uint16_t third = hal_sonar_cm();

    return (first + second + third) / 3;
}
