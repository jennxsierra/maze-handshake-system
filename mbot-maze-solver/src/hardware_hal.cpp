#include "hardware_hal.h"
#include "MeMegaPi.h"

namespace {
    // RGB LED
    MeRGBLed g_led{PORT_6};
    constexpr uint8_t LED_COUNT{4};

    // Motors (MegaPi onboard motor channels)
    MeMegaPiDCMotor g_left{PORT3B};
    MeMegaPiDCMotor g_right{PORT2B};

    // Ultrasonic
    MeUltrasonicSensor g_sonar{PORT_7};

    // Line follower
    MeLineFollower g_line{PORT_8};
}

namespace Hardware {
    void initBle(long baud) {
        Serial3.begin(baud);
    }

    bool bleAvailable() {
        return Serial3.available() > 0;
    }

    char bleRead() {
        return Serial3.available() ? (char)Serial3.read() : 0;
    }

    void bleWrite(char c) {
        Serial3.write(c);
    }

    void setRgb(uint8_t r, uint8_t g, uint8_t b) {
        for (uint8_t i = 0; i < LED_COUNT; ++i) {
            g_led.setColorAt(i, r, g, b);
        }

        g_led.show();
    }

    void setMotors(int left, int right) {
        g_left.run(left);
        g_right.run(-right); // right motor flipped on chassis
    }

    uint16_t readUltrasonicCm() {
        return g_sonar.distanceCm();
    }

    uint8_t readLineSensors() {
        return g_line.readSensors();
    }
}
