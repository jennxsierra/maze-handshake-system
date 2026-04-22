#pragma once
#include <Arduino.h>

class DriveSystem {
public:
    static constexpr int CRUISE = 110;

    void forward(int speed = CRUISE);
    void backward(int speed = CRUISE);
    void spinLeft(int speed);
    void spinRight(int speed);
    void stop();

    void turnLeft90();
    void turnRight90();
    void turnAround();

    // proportional wall-following
    void forwardWithCorrection(uint16_t currentCm, uint16_t targetCm, float gain);

private:
    void apply(int left, int right);

    static constexpr int L90_SPEED{ 200 };
    static constexpr int L90_MS{ 175 };
    static constexpr int R90_SPEED{ 225 };
    static constexpr int R90_MS{ 225 };
    static constexpr int UTURN_SPEED{ 200 };
    static constexpr int UTURN_MS{ 500 };
};
