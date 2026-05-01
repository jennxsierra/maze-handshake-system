#include "RgbStatusLight.h"

// Hardware-layer function from hal_hardware.cpp
void hal_rgb_set(uint8_t red, uint8_t green, uint8_t blue);

void RgbStatusLight::red() {
    setColor(255, 0, 0);
}

void RgbStatusLight::green() {
    setColor(0, 255, 0);
}

void RgbStatusLight::blue() {
    setColor(0, 0, 255);
}

void RgbStatusLight::off() {
    setColor(0, 0, 0);
}

void RgbStatusLight::setColor(uint8_t red, uint8_t green, uint8_t blue) {
    hal_rgb_set(red, green, blue);
}
