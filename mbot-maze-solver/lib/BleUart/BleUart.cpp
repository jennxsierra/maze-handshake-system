#include "BleUart.h"

// Hardware-layer functions implemented in hal_hardware.cpp
void hal_ble_begin(long baudRate);
bool hal_ble_available();
char hal_ble_read();
void hal_ble_write(char c);

// begin the BLE UART transport with the specified baud rate (default 115200)
void BleUart::begin(long baudRate) {
    hal_ble_begin(baudRate);
}

// return true if there is data available to read from the BLE UART
bool BleUart::hasData() const {
    return hal_ble_available();
}

// read a single byte from the BLE UART
char BleUart::readByte() {
    return hal_ble_read();
}

// write a single byte to the BLE UART
void BleUart::writeByte(char c) {
    hal_ble_write(c);
}

// write a null-terminated string to the BLE UART
void BleUart::writeText(const char* text) {
    if (!text) {
        return;
    }

    while (*text) {
        hal_ble_write(*text++);
    }
}
