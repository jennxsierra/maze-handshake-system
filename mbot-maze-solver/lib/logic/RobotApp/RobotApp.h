#pragma once
#include "../control/CommsUART/CommsUart.h"
#include "../control/DriveSystem/DriveSystem.h"
#include "../control/Indicators/Indicators.h"
#include "../control/Sensors/Sensors.h"
#include "../logic/ManualControl/ManualControl.h"
#include "../logic/Navigator/Navigator.h"
#include "RobotState.h"

class RobotApp {
public:
    RobotApp();

    void setup();
    void loop();

private:
    void stepManual();
    void stepAuto();
    void showBanner();

    RobotState state{ RobotState::MANUAL };

    CommsUart comms;
    DriveSystem drive;
    Indicators leds;
    RangeSensor sonar;
    LineBarrier line;

    ManualControl manual;
    Navigator autoNav;

    static constexpr unsigned long LOOP_DELAY_MS{ 20 };
};
