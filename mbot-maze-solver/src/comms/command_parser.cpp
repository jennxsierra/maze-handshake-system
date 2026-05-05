#include "comms/command_parser.h"

namespace maze {

CommandParser::CommandParser(MazeSolver &solver, RobotHardware &hardware, DriveBase &drive, ManualController &motors)
  : solver(solver), hardware(hardware), drive(drive), motors(motors), rgbLedOn_(false) {}

void CommandParser::parseCommand(char command) {
  switch (command) {
  case 'X':
    rgbLedOn_ = !rgbLedOn_;
    if (rgbLedOn_) {
      hardware.setStatusColor(0, 255, 0); // set GREEN
    } else {
      hardware.clearStatusColor();
    }
    break;
  case 'F':
    solver.setMode(MazeSolver::Mode::Manual);
    motors.driveForward(210);
    break;
  case 'B':
    solver.setMode(MazeSolver::Mode::Manual);
    motors.driveBackward(210);
    break;
  case 'L':
    solver.setMode(MazeSolver::Mode::Manual);
    motors.pivotLeft90();
    break;
  case 'R':
    solver.setMode(MazeSolver::Mode::Manual);
    motors.pivotRight90();
    break;
  case '1':
    solver.setMode(MazeSolver::Mode::Manual);
    motors.pivotLeft(95);
    break;
  case '2':
    solver.setMode(MazeSolver::Mode::Manual);
    motors.pivotRight(95);
    break;
  case 'S':
    drive.stop();
    break;
  case 'A':
    solver.setMode(MazeSolver::Mode::Autonomous);
    break;
  case 'I':
    solver.setMode(MazeSolver::Mode::Autonomous);
    break;
  case 'M':
    solver.setMode(MazeSolver::Mode::Manual);
    break;
  default:
    break;
  }
}

} // namespace maze