#include "comms/command_parser.h"

namespace maze
{
  // Initialize with references to the robot subsystems the parser controls.
  CommandParser::CommandParser(MazeSolver &solver, RobotHardware &hardware, DriveBase &drive, ManualController &motors)
      : solver(solver), hardware(hardware), drive(drive), motors(motors), rgbLedOn_(false) {}

  // Map single-character commands to actions.
  void CommandParser::parseCommand(char command)
  {
    switch (command)
    {
    case 'X':
      // Toggle status LED on/off.
      rgbLedOn_ = !rgbLedOn_;
      if (rgbLedOn_)
      {
        hardware.setStatusColor(0, 255, 0); // set GREEN
      }
      else
      {
        hardware.clearStatusColor();
      }
      break;

    // Manual movement: ensure solver in Manual mode then drive accordingly.
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
      // Immediate stop of drive base.
      drive.stop();
      break;

    // Mode switches: set solver to autonomous or manual.
    case 'A':
      solver.setMode(MazeSolver::Mode::Autonomous);
      break;
    case 'M':
      solver.setMode(MazeSolver::Mode::Manual);
      break;

    default:
      // Unknown command: ignore.
      break;
    }
  }
}
