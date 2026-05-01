#pragma once

#include "BleUart.h"
#include "DriveBase.h"
#include "RightHandNavigator.h"
#include "RgbStatusLight.h"
#include "RobotState.h"

// ManualCommandRouter interprets incoming command bytes while the robot is in manual mode.
class ManualCommandRouter {
public:
    ManualCommandRouter(BleUart& ble, RgbStatusLight& light, DriveBase& drive, RobotState& state, RightHandNavigator& navigator)
        : ble(ble), light(light), drive(drive), state(state), navigator(navigator), lightEnabled(false) {}

    void handle(char command);

private:
    BleUart& ble;
    RgbStatusLight& light;
    DriveBase& drive;
    RobotState& state;
    RightHandNavigator& navigator;
    bool lightEnabled{ false };
};
