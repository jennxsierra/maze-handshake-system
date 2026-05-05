#include "algorithm/solver.h"
#include "config/config.h"

namespace maze
{
  // Initialize solver with hardware/drive references and reset all state.
  MazeSolver::MazeSolver(RobotHardware &hardware, DriveBase &drive)
      : hardware(hardware),
        drive(drive),
        mode_(Mode::Manual),
        insideMode_(false),
        turned_(false),
        islandEntryChecksRemaining_(0),
        completed_(false),
        consecutiveLeftTurns_(0) {}

  // Initialize hardware and ensure motors are stopped.
  void MazeSolver::begin()
  {
    hardware.clearStatusColor();
    drive.stop();
  }

  // Main loop: detect island entry, read sensors, and dispatch movement logic.
  void MazeSolver::update()
  {
    // Skip if maze completed or in manual mode.
    if (completed_ || mode_ == Mode::Manual)
    {
      return;
    }

    // Island entry: after 4 left turns, turnaround and enter island mode.
    if (consecutiveLeftTurns_ == 4 && !turned_)
    {
      hardware.setStatusColor(0, 0, 255);
      drive.turnAround();
      turned_ = true;
      insideMode_ = true;
      islandEntryChecksRemaining_ = 2;
      delay(config::kPauseIslandSettle);
    }

    // Read all sensors.
    const uint8_t front = hardware.readFrontLine();
    const double rightDistanceCm = hardware.readRightDistanceCm();
    const bool rightInvalid = (static_cast<int>(rightDistanceCm) == config::kRightInvalidReading);
    const bool rightBlocked = hardware.isRightBlocked(rightDistanceCm) || rightInvalid;

    // Dispatch to appropriate movement handler.
    if (!insideMode_)
    {
      basicMovement(front, rightDistanceCm, rightBlocked);
    }
    else
    {
      islandMovement(front, rightDistanceCm, rightBlocked);
    }
  }

  // Switch between autonomous and manual modes. Stops drive when entering manual.
  void MazeSolver::setMode(Mode mode)
  {
    mode_ = mode;
    if (mode_ == Mode::Manual)
    {
      drive.stop();
    }
    else if (mode_ == Mode::Autonomous)
    {
      // Allow re-entering autonomous mode after completion by clearing
      // the completed flag and resetting transient navigation state.
      completed_ = false;
      insideMode_ = false;
      turned_ = false;
      consecutiveLeftTurns_ = 0;
    }
  }

  // Return current solver mode.
  MazeSolver::Mode MazeSolver::mode() const
  {
    return mode_;
  }

  // Return whether maze solving is complete.
  bool MazeSolver::isCompleted() const
  {
    return completed_;
  }

  // Basic maze navigation: wall-follow on right, left-turn on obstacles.
  void MazeSolver::basicMovement(uint8_t front, double rightDistanceCm, bool rightBlocked)
  {
    // Obstacle ahead: turn left and increment left-turn counter (triggers island at 4).
    if (front != 0)
    {
      drive.stop();
      consecutiveLeftTurns_++;
      drive.turnLeft();
      return;
    }

    // Right wall opened: move right to explore.
    if (!rightBlocked)
    {
      consecutiveLeftTurns_ = 0;
      turned_ = false;
      drive.moveRight();
      return;
    }

    // Follow wall: apply distance-based adjustments.
    applyWallFollowAdjustment(rightDistanceCm);
  }

  // Island navigation: stabilize on entry, then continue wall-following until exit.
  void MazeSolver::islandMovement(uint8_t front, double rightDistanceCm, bool rightBlocked)
  {
    // Entry phase: stabilize on right wall for a few cycles.
    if (islandEntryChecksRemaining_ > 0)
    {
      islandEntryChecksRemaining_--;

      // If wall opened, reorient aggressively if very far.
      if (!rightBlocked)
      {
        if (shouldAggressiveIslandRecentering(rightDistanceCm))
        {
          drive.adjustRightVeryStrong();
        }
        else
        {
          drive.moveForward();
        }
        return;
      }

      // Apply stabilization to find right wall.
      applyIslandEntryStabilization(rightDistanceCm);
      return;
    }

    // Exit condition: front obstacle detected after island.
    if (front != 0)
    {
      markCompleted();
      return;
    }

    // Right wall opened: move right to explore.
    if (!rightBlocked)
    {
      consecutiveLeftTurns_ = 0;
      drive.moveRight();
      return;
    }

    // Follow wall: apply standard wall-following adjustments.
    applyWallFollowAdjustment(rightDistanceCm);
  }

  // Adjust motion based on right-wall distance: keep robot parallel to wall.
  void MazeSolver::applyWallFollowAdjustment(double rightDistanceCm)
  {
    // Treat sensor sentinel as "invalid/too-close" and force strong left.
    if (static_cast<int>(rightDistanceCm) == config::kRightInvalidReading)
    {
      drive.adjustLeftStrong();
      return;
    }
    // Too close to right wall: push left.
    if (rightDistanceCm <= config::kCloseStrongCm)
    {
      drive.adjustLeftStrong();
    }
    else if (rightDistanceCm <= config::kCloseLightCm)
    {
      drive.adjustLeft();
      // Slightly far from wall: nudge right.
    }
    else if (rightDistanceCm >= config::kFarLightLowCm &&
             rightDistanceCm <= config::kFarLightHighCm)
    {
      drive.adjustRight();
      // Far from wall: strong push right.
    }
    else if (rightDistanceCm > config::kFarStrongLowCm &&
             rightDistanceCm <= config::kFarStrongHighCm)
    {
      drive.adjustRightStrong();
      // Very far from wall: very strong push right.
    }
    else if (rightDistanceCm > config::kFarVeryStrongLowCm &&
             rightDistanceCm <= config::kFarVeryStrongHighCm)
    {
      drive.adjustRightVeryStrong();
      // In target zone: go straight.
    }
    else
    {
      drive.moveForward();
    }
  }

  // During island entry, apply conservative adjustments to find and stabilize on right wall.
  void MazeSolver::applyIslandEntryStabilization(double rightDistanceCm)
  {
    // Treat sentinel as invalid -> strong left to reorient.
    if (static_cast<int>(rightDistanceCm) == config::kRightInvalidReading)
    {
      drive.adjustLeftStrong();
      return;
    }
    // Too close: push left.
    if (rightDistanceCm <= config::kCloseStrongCm)
    {
      drive.adjustLeftStrong();
    }
    else if (rightDistanceCm <= config::kCloseLightCm)
    {
      drive.adjustLeft();
      // Far from wall: strong push right to find it.
    }
    else if (rightDistanceCm > config::kFarStrongLowCm)
    {
      drive.adjustRightStrong();
      // In target zone: go straight.
    }
    else
    {
      drive.moveForward();
    }
  }

  // Determine if robot is far enough from right wall to warrant aggressive recentering.
  bool MazeSolver::shouldAggressiveIslandRecentering(double rightDistanceCm) const
  {
    return rightDistanceCm > config::kFarVeryStrongLowCm;
  }

  // Mark maze as complete: stop motors, set LED green, and log completion.
  void MazeSolver::markCompleted()
  {
    drive.stop();
    hardware.setStatusColor(0, 255, 0);
    completed_ = true;
    mode_ = Mode::Manual;
    Serial.println("===== MAZE COMPLETED =====");
  }
}
