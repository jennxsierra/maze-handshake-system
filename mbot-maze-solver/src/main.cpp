#include <Arduino.h>

#include "BleUart.h"
#include "DriveBase.h"
#include "LineFollower.h"
#include "ManualCommandRouter.h"
#include "RightHandNavigator.h"
#include "RgbStatusLight.h"
#include "RobotState.h"
#include "SonarRange.h"

// Main loop timing.
constexpr unsigned long kLoopDelayMs{ 12 };

// Global operating mode.
RobotState gState{ RobotState::MANUAL };

// Hardware-facing objects.
BleUart gBle;
LineFollower gLine;
DriveBase gDrive;
RgbStatusLight gLight;
SonarRange gSonar;

// High-level behaviors.
RightHandNavigator gNavigator{ gBle, gLine, gDrive, gLight, gSonar };
ManualCommandRouter gRouter{ gBle, gLight, gDrive, gState, gNavigator };

// Print startup status lines to the serial monitor.
static void printBanner() {
    Serial.println("[maze-runner] robot maze solver ready");
    Serial.println("[mode] manual and autonomous control supported");
    Serial.println();
}

void setup() {
    // Initialize debug serial and BLE command link.
    Serial.begin(9600);
    gBle.begin(115200);
    printBanner();
}

void loop() {
    // In manual mode, consume and route all pending BLE commands.
    if (gState == RobotState::MANUAL) {
        while (gBle.hasData()) {
            gRouter.handle(gBle.readByte());
        }
    } else {
        // Manual override while running autonomously.
        if (gBle.hasData()) {
            char command = gBle.readByte();
            if (command == 'M') {
                gDrive.stop();
                gNavigator.reset();
                gState = RobotState::MANUAL;
                gBle.writeText("[AUTO] overriding to manual mode\n");
                return;
            }
        }

        // Run one autonomous navigation step.
        gNavigator.update();

        // Hand control back to manual mode after the goal is reached.
        if (gNavigator.goalReached()) {
            gState = RobotState::MANUAL;
            gBle.writeText("[AUTO] goal reached; returning to manual mode\n");
            return;
        }
    }

    // Keep a short, stable loop cadence.
    delay(kLoopDelayMs);
}
