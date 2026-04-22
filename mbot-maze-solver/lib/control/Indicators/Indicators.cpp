#include "Indicators.h"
#include "../../../src/hardware_hal.h"

void Indicators::red()   { set(255, 0, 0); }
void Indicators::green() { set(0, 255, 0); }
void Indicators::blue()  { set(0, 0, 255); }
void Indicators::off()   { set(0, 0, 0); }

void Indicators::set(uint8_t r, uint8_t g, uint8_t b) {
    Hardware::setRgb(r, g, b);
}
