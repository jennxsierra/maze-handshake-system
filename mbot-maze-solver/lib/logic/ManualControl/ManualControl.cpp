#include "ManualControl.h"

const ManualControl::Entry ManualControl::table[] = {
    { 'X', &ManualControl::toggleLamp },
    { 'F', &ManualControl::fwd },
    { 'B', &ManualControl::back },
    { 'L', &ManualControl::left90 },
    { 'R', &ManualControl::right90 },
    { '1', &ManualControl::softLeft },
    { '2', &ManualControl::softRight },
    { 'S', &ManualControl::halt },
    { 'A', &ManualControl::setAuto },
    { 'M', &ManualControl::confirmManual }
};

ManualControl::ManualControl(CommsUart& io, DriveSystem& drive, Indicators& leds, RobotState& state)
    : io(io), drive(drive), leds(leds), state(state) {}

void ManualControl::handle(char cmd) {
    for (const auto& e : table) {
        if (e.key == cmd) {
            (this->*e.action)();
            return;
        }
    }
    unknown();
}

void ManualControl::toggleLamp() {
    if (lampOn) leds.off();
    else leds.green();
    lampOn = !lampOn;
}

void ManualControl::fwd()       { drive.forward(150); }
void ManualControl::back()      { drive.backward(150); }
void ManualControl::left90()    { drive.turnLeft90(); }
void ManualControl::right90()   { drive.turnRight90(); }
void ManualControl::softLeft()  { drive.spinLeft(55); }
void ManualControl::softRight() { drive.spinRight(55); }
void ManualControl::halt()      { drive.stop(); }

void ManualControl::setAuto() {
    state = RobotState::AUTO;
    io.write("[MANUAL] -> AUTO\n");
}

void ManualControl::confirmManual() {
    io.write("[MANUAL] Already manual\n");
}

void ManualControl::unknown() {
    io.write("[MANUAL] Unknown command\n");
}
