#pragma once
#include <Arduino.h>

#include "BleUart.h"
#include "DriveBase.h"
#include "LineFollower.h"
#include "RgbStatusLight.h"
#include "SonarRange.h"

// RightHandNavigator directs a robot to solve a maze using the right-hand rule.
class RightHandNavigator {
public:
    // Constructor
    RightHandNavigator(BleUart& ble, LineFollower& line, DriveBase& drive, RgbStatusLight& light, SonarRange& sonar);

    // Update keeps track of the maze state and directs the appropriate robot movement.
    void update();
    // Check if the designated finish point has been reached.
    bool goalReached() const;
    // Revert all flags, counters, and values to their initial states.
    void reset();
    // Revert all maze solver state except for the RGB LED.
    void resetAfterGoal();
    // Enable ISLAND state for the maze solver.
    void setIslandState();

private:
    // Navigation Mode that determines logic flow in update()
    enum class State { NORMAL, ISLAND };
    State currentState{ State::NORMAL };

    // Hardware Components
    BleUart& ble;
    LineFollower& line;
    DriveBase& drive;
    RgbStatusLight& light;
    SonarRange& sonar;

    // Counters & Flags & Configurations
    bool done{ false };
    bool rightTurnCooldown{ false };
    int wallDetectionCounter{ 0 };
    
    static constexpr int ISLAND_DETECTION_THRESHOLD{ 4 };
    static constexpr int RIGHT_OPENING_THRESHOLD{ 18 };
    static constexpr int RIGHT_WALL_TARGET{ 6 };
    static constexpr float PROPORTIONAL_GAIN{ 3.0f };

    // Helper Methods
    void compensateForSensorOffset();
    void compensateForTurnOffset();
    void compensateFor180TurnOffset();
    void retreatFromWall();
    void pauseAfterTurn();
};
