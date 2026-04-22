#pragma once

class CommsUart {
public:
    void begin(long baud = 115200);
    bool available() const;
    char read();
    void write(char c);
    void write(const char* text);
};
