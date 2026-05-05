#include "algorithm/solver.h"
#include "config/config.h"

namespace maze {

MazeSolver::MazeSolver(RobotHardware &hardware, DriveBase &drive)
    : hardware(hardware),
      drive(drive),
      mode_(Mode::Manual),
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
    delay(config::kPauseIslandSettle);
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

  applyWallFollowAdjustment(rightDistanceCm);
}

void MazeSolver::islandMovement(uint8_t front, double rightDistanceCm, bool rightBlocked) {
  if (islandEntryChecksRemaining_ > 0) {
    islandEntryChecksRemaining_--;

    if (!rightBlocked) {
      if (shouldAggressiveIslandRecentering(rightDistanceCm)) {
        drive.adjustRightVeryStrong();
      } else {
        drive.moveForward();
      }
      return;
    }

    applyIslandEntryStabilization(rightDistanceCm);
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

  applyWallFollowAdjustment(rightDistanceCm);
}

void MazeSolver::applyWallFollowAdjustment(double rightDistanceCm) {
  if (rightDistanceCm <= config::kCloseStrongCm) {
    drive.adjustLeftStrong();
  } else if (rightDistanceCm <= config::kCloseLightCm) {
    drive.adjustLeft();
  } else if (rightDistanceCm >= config::kFarLightLowCm &&
             rightDistanceCm <= config::kFarLightHighCm) {
    drive.adjustRight();
  } else if (rightDistanceCm > config::kFarStrongLowCm &&
             rightDistanceCm <= config::kFarStrongHighCm) {
    drive.adjustRightStrong();
  } else if (rightDistanceCm > config::kFarVeryStrongLowCm &&
             rightDistanceCm <= config::kFarVeryStrongHighCm) {
    drive.adjustRightVeryStrong();
  } else {
    drive.moveForward();
  }
}

void MazeSolver::applyIslandEntryStabilization(double rightDistanceCm) {
  if (rightDistanceCm <= config::kCloseStrongCm) {
    drive.adjustLeftStrong();
  } else if (rightDistanceCm <= config::kCloseLightCm) {
    drive.adjustLeft();
  } else if (rightDistanceCm > config::kFarStrongLowCm) {
    drive.adjustRightStrong();
  } else {
    drive.moveForward();
  }
}

bool MazeSolver::shouldAggressiveIslandRecentering(double rightDistanceCm) const {
  return rightDistanceCm > config::kFarVeryStrongLowCm;
}

void MazeSolver::markCompleted() {
  drive.stop();
  hardware.setStatusColor(0, 255, 0);
  completed_ = true;
  Serial.println("===== MAZE COMPLETED =====");
}

} // namespace maze