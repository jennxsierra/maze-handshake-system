#include "RightHandNavigator.h"

RightHandNavigator::RightHandNavigator(BleUart& ble, LineFollower& line, DriveBase& drive, RgbStatusLight& light, SonarRange& sonar)
    : ble(ble), line(line), drive(drive), light(light), sonar(sonar) {}

void RightHandNavigator::update() {
    // Read sensor values.
    uint16_t sensorDistance = sonar.centimeters();
    bool rightWallOpen = sensorDistance >= RIGHT_OPENING_THRESHOLD;
    bool frontObstructed = line.frontBlocked();

    // Manage right turn cooldown state.
    if (rightTurnCooldown) {
        rightTurnCooldown = rightWallOpen ? false : true;
    }

    // Route behavior based on current navigation mode.
    switch (currentState) {
    case State::NORMAL: {
        // Right opening detected - execute right turn sequence.
        if (!rightWallOpen && !rightTurnCooldown) {
            wallDetectionCounter = 0;
            rightTurnCooldown = true;

            ble.writeText("[NORMAL] turning right\n");
            compensateForSensorOffset();
            drive.turnRight90();
            pauseAfterTurn();
            compensateForTurnOffset();
            break;
        }

        // Front blocked - evaluate wall streak for island detection.
        if (frontObstructed) {
            ++wallDetectionCounter;

            if (wallDetectionCounter >= ISLAND_DETECTION_THRESHOLD) {
                // Island detected - transition to inner path mode.
                currentState = State::ISLAND;
                rightTurnCooldown = true;
                light.red();

                ble.writeText("[ISLAND] mode activated\n");
                drive.stop();
                retreatFromWall();
                drive.turnAround();
                compensateFor180TurnOffset();
            } else {
                // Execute left turn.
                ble.writeText("[NORMAL] turning left\n");
                drive.stop();
                retreatFromWall();
                drive.turnLeft90();
            }
            break;
        }

        // Move forward with or without correction based on cooldown state.
        if (rightTurnCooldown) {
            drive.forward();
        } else {
            drive.forwardWithCorrection(sensorDistance, RIGHT_WALL_TARGET, PROPORTIONAL_GAIN);
        }
        break;
    }

    case State::ISLAND: {
        // Right opening in island mode - execute right turn sequence.
        if (!rightWallOpen && !rightTurnCooldown) {
            rightTurnCooldown = true;

            ble.writeText("[ISLAND] turning right\n");
            drive.stop();
            compensateForSensorOffset();
            drive.turnRight90();
            pauseAfterTurn();
            compensateForTurnOffset();
            break;
        }

        // Front blocked in island mode - goal reached.
        if (frontObstructed) {
            drive.stop();
            retreatFromWall();
            drive.stop();
            light.green();
            done = true;
            break;
        }

        // Move forward with or without correction based on cooldown state.
        if (rightTurnCooldown) {
            drive.forward();
        } else {
            drive.forwardWithCorrection(sensorDistance, RIGHT_WALL_TARGET, PROPORTIONAL_GAIN);
        }
        break;
    }
    }
}

bool RightHandNavigator::goalReached() const {
    return done;
}

void RightHandNavigator::reset() {
    currentState = State::NORMAL;
    done = false;
    rightTurnCooldown = false;
    wallDetectionCounter = 0;
    light.off();
}

void RightHandNavigator::resetAfterGoal() {
    currentState = State::NORMAL;
    done = false;
    rightTurnCooldown = false;
    wallDetectionCounter = 0;
}

void RightHandNavigator::setIslandState() {
    currentState = State::ISLAND;
}

// Offset compensation after detecting sensor position on right turn.
void RightHandNavigator::compensateForSensorOffset() {
    drive.forward();
    delay(725);
}

// Offset compensation after completing a right turn.
void RightHandNavigator::compensateForTurnOffset() {
    drive.forward();
    delay(725);
}

// Offset compensation after completing a 180-degree turn.
void RightHandNavigator::compensateFor180TurnOffset() {
    drive.forward();
    delay(1000);
}

// Reverse slightly to avoid wall contact during turns.
void RightHandNavigator::retreatFromWall() {
    drive.backward();
    delay(200);
}

// Stop and pause after directional turn.
void RightHandNavigator::pauseAfterTurn() {
    drive.stop();
    delay(300);
}
