#pragma once

#include <Arduino.h>

#include "motion/drive.h"
#include "hardware/hardware.h"
#include "algorithm/solver.h"
#include "motion/manual_controller.h"

namespace maze
{
  // Centralizes and maps single-character commands (USB/BLE) to robot actions.
  // Responsible for toggling modes, directly driving motors, and setting status LED.
  class CommandParser
  {
  public:
    CommandParser(MazeSolver &solver, RobotHardware &hardware, DriveBase &drive, ManualController &motors);

    void parseCommand(char command);

  private:
    MazeSolver &solver;
    RobotHardware &hardware;
    DriveBase &drive;
    ManualController &motors;
    bool rgbLedOn_;
  };
}
