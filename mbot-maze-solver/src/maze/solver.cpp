#include "maze/solver.h"

#include "maze/config.h"

namespace maze {

MazeSolver::MazeSolver(RobotHardware &hardware, DriveBase &drive)
    : hardware(hardware),
      drive(drive),
      mode_(Mode::Autonomous),
      insideMode_(false),
      turned_(false),
      islandEntryChecksRemaining_(0),
      completed_(false),
      consecutiveLeftTurns_(0) {}

void MazeSolver::begin() {
  hardware.clearStatusColor();
  drive.stop();
}

void MazeSolver::update() {
  if (completed_ || mode_ == Mode::Manual) {
    return;
  }

  if (consecutiveLeftTurns_ == 4 && !turned_) {
    hardware.setStatusColor(0, 0, 255);
    drive.turnAround();
    turned_ = true;
    insideMode_ = true;
    islandEntryChecksRemaining_ = 2;
    delay(config::kPauseIslandSettleMs);
  }

  const uint8_t front = hardware.readFrontLine();
  const double rightDistanceCm = hardware.readRightDistanceCm();
  const bool rightBlocked = hardware.isRightBlocked(rightDistanceCm);

  if (!insideMode_) {
    basicMovement(front, rightDistanceCm, rightBlocked);
  } else {
    islandMovement(front, rightDistanceCm, rightBlocked);
  }
}

void MazeSolver::setMode(Mode mode) {
  mode_ = mode;
  if (mode_ == Mode::Manual) {
    drive.stop();
  }
}

MazeSolver::Mode MazeSolver::mode() const {
  return mode_;
}

bool MazeSolver::isCompleted() const {
  return completed_;
}

void MazeSolver::handleCommand(char command) {
  switch (command) {
  case 'A':
    setMode(Mode::Autonomous);
    break;
  case 'M':
    setMode(Mode::Manual);
    break;
  case 'S':
    drive.stop();
    break;
  case 'F':
    setMode(Mode::Manual);
    drive.moveForward();
    break;
  case 'B':
    setMode(Mode::Manual);
    drive.moveBackward();
    break;
  case 'L':
    setMode(Mode::Manual);
    drive.turnLeft();
    break;
  case 'R':
    setMode(Mode::Manual);
    drive.moveRight();
    break;
  case '1':
    setMode(Mode::Manual);
    drive.readjustLeftwards();
    break;
  case '2':
    setMode(Mode::Manual);
    drive.readjustRightwards();
    break;
  default:
    break;
  }
}

void MazeSolver::basicMovement(uint8_t front, double rightDistanceCm, bool rightBlocked) {
  if (front != 0) {
    drive.stop();
    consecutiveLeftTurns_++;
    drive.turnLeft();
    return;
  }

  if (!rightBlocked) {
    consecutiveLeftTurns_ = 0;
    turned_ = false;
    drive.moveRight();
    return;
  }

  if (rightDistanceCm <= config::kTooCloseStrongCm) {
    drive.readjustLeftwardsStrong();
  } else if (rightDistanceCm <= config::kTooCloseLightCm) {
    drive.readjustLeftwards();
  } else if (rightDistanceCm >= config::kTooFarLightLowCm &&
             rightDistanceCm <= config::kTooFarLightHighCm) {
    drive.readjustRightwards();
  } else if (rightDistanceCm > config::kTooFarStrongLowCm &&
             rightDistanceCm <= config::kTooFarStrongHighCm) {
    drive.readjustRightwardsStrong();
  } else if (rightDistanceCm > config::kTooFarVeryStrongLowCm &&
             rightDistanceCm <= config::kTooFarVeryStrongHighCm) {
    drive.readjustRightwardsVeryStrong();
  } else {
    drive.moveForward();
  }
}

void MazeSolver::islandMovement(uint8_t front, double rightDistanceCm, bool rightBlocked) {
  if (islandEntryChecksRemaining_ > 0) {
    // During initial checks after entering the island we need to handle
    // two cases:
    // 1) A real opening exists immediately -> move forward.
    // 2) No opening, but the robot is too far from the right wall after the
    //    turn -> aggressively readjust rightwards so subsequent readings
    //    correctly detect the wall and avoid false positives.
    islandEntryChecksRemaining_--;

    if (!rightBlocked) {
      if (rightDistanceCm > config::kTooFarVeryStrongLowCm) {
        // Too far from the right wall after the turn; nudge rightwards hard
        // so we can sense the wall reliably on the next updates.
        drive.readjustRightwardsVeryStrong();
      } else {
        // Likely a genuine opening; proceed forward into it.
        drive.moveForward();
      }
      return;
    }

    // During entry checks, never idle: keep issuing a stabilizing command
    // while we re-lock to the right wall.
    if (rightDistanceCm <= config::kTooCloseStrongCm) {
      drive.readjustLeftwardsStrong();
    } else if (rightDistanceCm <= config::kTooCloseLightCm) {
      drive.readjustLeftwards();
    } else if (rightDistanceCm > config::kTooFarStrongLowCm) {
      drive.readjustRightwardsStrong();
    } else {
      drive.moveForward();
    }
    return;
  }

  if (front != 0) {
    markCompleted();
    return;
  }

  if (!rightBlocked) {
    consecutiveLeftTurns_ = 0;
    drive.moveRight();
    return;
  }

  if (rightDistanceCm <= config::kTooCloseStrongCm) {
    drive.readjustLeftwardsStrong();
  } else if (rightDistanceCm <= config::kTooCloseLightCm) {
    drive.readjustLeftwards();
  } else if (rightDistanceCm >= config::kTooFarLightLowCm &&
             rightDistanceCm <= config::kTooFarLightHighCm) {
    drive.readjustRightwards();
  } else if (rightDistanceCm > config::kTooFarStrongLowCm &&
             rightDistanceCm <= config::kTooFarStrongHighCm) {
    drive.readjustRightwardsStrong();
  } else if (rightDistanceCm > config::kTooFarVeryStrongLowCm &&
             rightDistanceCm <= config::kTooFarVeryStrongHighCm) {
    drive.readjustRightwardsVeryStrong();
  } else {
    drive.moveForward();
  }
}

void MazeSolver::markCompleted() {
  drive.stop();
  hardware.setStatusColor(0, 255, 0);
  completed_ = true;
  Serial.println("===== MAZE COMPLETED =====");
}

} // namespace maze