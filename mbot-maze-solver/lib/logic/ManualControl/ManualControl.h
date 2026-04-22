#pragma once
#include "../control/CommsUART/CommsUart.h"
#include "../control/DriveSystem/DriveSystem.h"
#include "../control/Indicators/Indicators.h"
#include "RobotState.h"

// ManualControl is a table-driven dispatcher for control characters.
class ManualControl {
public:
    ManualControl(CommsUart& io, DriveSystem& drive, Indicators& leds, RobotState& state);

    void handle(char cmd);

private:
    struct Entry {
        char key;
        void (ManualControl::*action)();
    };

    void toggleLamp();
    void fwd();
    void back();
    void left90();
    void right90();
    void softLeft();
    void softRight();
    void halt();
    void setAuto();
    void confirmManual();
    void unknown();

    CommsUart& io;
    DriveSystem& drive;
    Indicators& leds;
    RobotState& state;
    bool lampOn{ false };

    static const Entry table[];
};
