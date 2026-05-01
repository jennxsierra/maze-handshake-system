#pragma once
#include <Arduino.h>

// BleUart is a small wrapper around the BLE serial transport.
class BleUart {
public:
    void begin(long baudRate = 115200);
    bool hasData() const;
    char readByte();
    void writeByte(char c);
    void writeText(const char* text);
};
