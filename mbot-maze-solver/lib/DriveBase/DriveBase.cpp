#include "DriveBase.h"

// Hardware-layer function from hal_hardware.cpp
void hal_motor_set(int leftSpeed, int rightSpeed);

// Calculate correction factor and apply differential speeds to maintain target distance.
void DriveBase::forwardWithCorrection(uint16_t distanceCm, uint16_t targetCm, float gain) {
    // Static smoothing filter for stable motor control
    static float smoothedCorrection = 0;
    
    // Compute raw error
    int error = (int)targetCm - (int)distanceCm;
    
    // Reject negligible errors to reduce jitter
    if (abs(error) < 2) {
        smoothedCorrection = 0;
        apply(NOMINAL_SPEED, NOMINAL_SPEED);
        return;
    }
    
    // Compute proportional correction and clamp to safe range
    float rawCorrection = error * gain;
    rawCorrection = constrain(rawCorrection, -30.0f, 30.0f);
    
    // Apply exponential smoothing to filter noise and jerking
    smoothedCorrection = 0.7f * smoothedCorrection + 0.3f * rawCorrection;
    
    // Apply corrected motor speeds
    apply(NOMINAL_SPEED - (int)smoothedCorrection, NOMINAL_SPEED + (int)smoothedCorrection);
}

// Move forward at the specified speed (both motors equal).
void DriveBase::forward(int speed) {
    apply(speed, speed);
}

// Move backward at the specified speed (both motors negative).
void DriveBase::backward(int speed) {
    apply(-speed, -speed);
}

// Spin counterclockwise (left motor reverse, right motor forward).
void DriveBase::spinLeft(int speed) {
    apply(-speed, speed);
}

// Spin clockwise (left motor forward, right motor reverse).
void DriveBase::spinRight(int speed) {
    apply(speed, -speed);
}

// Execute a 90-degree left turn using timed rotation.
void DriveBase::turnLeft90() {
    spinLeft(LEFT_90_SPEED);
    delay(LEFT_90_MS);
    stop();
    delay(200);
}

// Execute a 90-degree right turn using timed rotation.
void DriveBase::turnRight90() {
    spinRight(RIGHT_90_SPEED);
    delay(RIGHT_90_MS);
    stop();
    delay(200);
}

// Execute a 180-degree U-turn using timed rotation.
void DriveBase::turnAround() {
    apply(-UTURN_SPEED, UTURN_SPEED);
    delay(UTURN_MS);
    stop();
    delay(200);
}

// Stop all motors.
void DriveBase::stop() {
    apply(0, 0);
}

// Send differential motor speeds to the hardware layer.
void DriveBase::apply(int leftSpeed, int rightSpeed) {
    hal_motor_set(leftSpeed, rightSpeed);
}
