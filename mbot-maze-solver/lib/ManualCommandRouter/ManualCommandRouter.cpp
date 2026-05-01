#include "ManualCommandRouter.h"

// handle routes a single incoming command to manual controls or mode changes.
void ManualCommandRouter::handle(char command) {
    switch (command) {
    // Toggle status light on/off.
    case 'X':
        if (lightEnabled) {
            light.off();
        } else {
            light.green();
        }
        lightEnabled = !lightEnabled;
        break;

    // Drive controls.
    case 'F':
        drive.forward(150);
        break;

    case 'B':
        drive.backward(150);
        break;

    case 'L':
        drive.turnLeft90();
        break;

    case 'R':
        drive.turnRight90();
        break;

    case '1':
        drive.spinLeft(55);
        break;

    case '2':
        drive.spinRight(55);
        break;

    case 'S':
        drive.stop();
        break;

    // Switch from manual to autonomous mode.
    case 'A':
        state = RobotState::AUTONOMOUS;
        ble.writeText("[MANUAL] switching to autonomous mode\n");
        break;

    // Prepare autonomous mode to start in island state.
    case 'I':
        navigator.setIslandState();
        ble.writeText("[MANUAL] setting island state for autonomous mode\n");
        break;

    // Manual mode status check.
    case 'M':
        ble.writeText("[MANUAL] already in manual mode\n");
        break;

    // Fallback for unsupported commands.
    default:
        ble.writeText("[MANUAL] unknown command\n");
        break;
    }
}
