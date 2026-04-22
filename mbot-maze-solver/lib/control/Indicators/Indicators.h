#pragma once
#include <Arduino.h>

class Indicators {
public:
    void red();
    void green();
    void blue();
    void off();

private:
    void set(uint8_t r, uint8_t g, uint8_t b);
};
