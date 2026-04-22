#include "RobotApp.h"
#include <Arduino.h>

RobotApp::RobotApp()
    : manual(comms, drive, leds, state),
      autoNav(comms, drive, leds, sonar, line) {}

void RobotApp::setup() {
    Serial.begin(9600);
    comms.begin(115200);
    showBanner();
}

void RobotApp::loop() {
    if (state == RobotState::MANUAL) stepManual();
    else stepAuto();

    delay(LOOP_DELAY_MS);
}

void RobotApp::stepManual() {
    while (comms.available()) {
        char c = comms.read();
        manual.handle(c);
    }
}

void RobotApp::stepAuto() {
    // allow manual override
    if (comms.available()) {
        char c = comms.read();
        if (c == 'M') {
            drive.stop();
            autoNav.reset();
            state = RobotState::MANUAL;
            comms.write("[AUTO] -> MANUAL\n");
            return;
        }
    }

    autoNav.tick();

    if (autoNav.goalReached()) {
        state = RobotState::MANUAL;
        comms.write("[AUTO] Goal reached\n");
        return;
    }
}

void RobotApp::showBanner() {
    Serial.println("[maze-app] Ready");
    Serial.println("[mode] manual or autonomous");
    Serial.println();
}
