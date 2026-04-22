#include "CommsUart.h"
#include "../../../src/hardware_hal.h"

void CommsUart::begin(long baud) {
    Hardware::initBle(baud);
}

bool CommsUart::available() const {
    return Hardware::bleAvailable();
}

char CommsUart::read() {
    return Hardware::bleRead();
}

void CommsUart::write(char c) {
    Hardware::bleWrite(c);
}

void CommsUart::write(const char* text) {
    while (text && *text) {
        Hardware::bleWrite(*text++);
    }
}
