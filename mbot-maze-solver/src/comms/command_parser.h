#pragma once

#include <Arduino.h>

#include "motion/drive.h"
#include "hardware/hardware.h"
#include "algorithm/solver.h"
#include "motion/manual_controller.h"

namespace maze {

class CommandParser {
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

} // namespace maze