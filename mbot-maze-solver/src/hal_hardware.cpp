#include "MeMegaPi.h"

/* RGB LED configuration */
MeRGBLed gRgbLed{ PORT_6 };
constexpr uint8_t kRgbCount{ 4 };

// Set all RGB LEDs to the specified color.
void hal_rgb_set(uint8_t red, uint8_t green, uint8_t blue) {
    for (uint8_t index = 0; index < kRgbCount; ++index) {
        gRgbLed.setColorAt(index, red, green, blue);
    }
    gRgbLed.show();
}

/* Motor configuration */
MeMegaPiDCMotor gLeftMotor{ PORT3B };
MeMegaPiDCMotor gRightMotor{ PORT2B };

void hal_motor_set(int leftSpeed, int rightSpeed) {
    gLeftMotor.run(leftSpeed);
    gRightMotor.run(-rightSpeed); // right motor is physically reversed
}

/* Ultrasonic sensor configuration */
MeUltrasonicSensor gSonar{ PORT_7 };

uint16_t hal_sonar_cm() {
    return gSonar.distanceCm();
}

/* Line follower configuration */
MeLineFollower gLineFollower{ PORT_8 };

uint8_t hal_line_read() {
    return gLineFollower.readSensors();
}

/* BLE serial transport */

// initialize BLE serial transport on Serial3 (RX3/TX3)
void hal_ble_begin(long baudRate) {
    Serial3.begin(baudRate);
}

// check if data is available on BLE serial transport
bool hal_ble_available() {
    return Serial3.available() > 0;
}

// read a byte from BLE serial transport, returns 0 if no data is available
char hal_ble_read() {
    if (Serial3.available() > 0) {
        return (char)Serial3.read();
    }
    return 0;
}

// write a byte to BLE serial transport
void hal_ble_write(char c) {
    Serial3.write(c);
}
