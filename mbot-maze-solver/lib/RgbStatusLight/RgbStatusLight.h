#pragma once
#include <Arduino.h>

// RgbStatusLight controls the RGB indicator on the robot.
class RgbStatusLight {
public:
    // Set the light to red, green, blue, or off.
    void red();
    void green();
    void blue();
    void off();

private:
    // Set the RGB color using the hardware-layer function.
    void setColor(uint8_t red, uint8_t green, uint8_t blue);
};
