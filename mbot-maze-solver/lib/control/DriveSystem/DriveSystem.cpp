#include "DriveSystem.h"
#include "../../../src/hardware_hal.h"

void DriveSystem::forward(int speed) {
    apply(speed, speed);
}

void DriveSystem::backward(int speed) {
    apply(-speed, -speed);
}

void DriveSystem::spinLeft(int speed) {
    apply(-speed, speed);
}

void DriveSystem::spinRight(int speed) {
    apply(speed, -speed);
}

void DriveSystem::stop() {
    apply(0, 0);
}

void DriveSystem::turnLeft90() {
    spinLeft(L90_SPEED);
    delay(L90_MS);
    stop();
    delay(200);
}

void DriveSystem::turnRight90() {
    spinRight(R90_SPEED);
    delay(R90_MS);
    stop();
    delay(200);
}

void DriveSystem::turnAround() {
    apply(-UTURN_SPEED, UTURN_SPEED);
    delay(UTURN_MS);
    stop();
    delay(200);
}

void DriveSystem::forwardWithCorrection(uint16_t currentCm, uint16_t targetCm, float gain) {
    int error = (int)targetCm - (int)currentCm;
    int adjust = (int)(error * gain);
    int left = CRUISE - adjust;
    int right = CRUISE + adjust;
    apply(left, right);
}

void DriveSystem::apply(int left, int right) {
    Hardware::setMotors(left, right);
}
