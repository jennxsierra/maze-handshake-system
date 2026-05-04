#pragma once

#include <Arduino.h>

#include "maze/drive.h"
#include "maze/hardware.h"
#include "maze/solver.h"
#include "maze/manual_controller.h"

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