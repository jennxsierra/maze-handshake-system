#pragma once
#include <Arduino.h>

// DriveBase exposes motion helpers for the two-wheel robot.
class DriveBase {
public:
    // Default motor speed for forward/backward movement.
    static constexpr int NOMINAL_SPEED{ 80 };

    // Move forward with feedback correction based on target distance.
    void forwardWithCorrection(uint16_t distanceCm, uint16_t targetCm, float gain);
    // Move forward at the specified speed.
    void forward(int speed = NOMINAL_SPEED);
    // Move backward at the specified speed.
    void backward(int speed = NOMINAL_SPEED);

    // Spin in place counterclockwise at the specified speed.
    void spinLeft(int speed);
    // Spin in place clockwise at the specified speed.
    void spinRight(int speed);

    // Perform a 90-degree left turn.
    void turnLeft90();
    // Perform a 90-degree right turn.
    void turnRight90();
    // Perform a 180-degree turn (U-turn).
    void turnAround();

    // Stop all motors.
    void stop();

private:
    // Speed and timing constants for 90-degree left turn.
    static constexpr int LEFT_90_SPEED{ 220 };
    static constexpr int LEFT_90_MS{ 120 };

    // Speed and timing constants for 90-degree right turn.
    static constexpr int RIGHT_90_SPEED{ 220 };
    static constexpr int RIGHT_90_MS{ 160 };

    // Speed and timing constants for 180-degree U-turn.
    static constexpr int UTURN_SPEED{ 200 };
    static constexpr int UTURN_MS{ 450 };

    // Apply differential motor speeds to both wheels (left and right).
    void apply(int leftSpeed, int rightSpeed);
};
