#pragma once

#include <Arduino.h>

#include "motion/drive.h"

namespace maze {

class MazeSolver {
public:
  enum class Mode {
    Autonomous,
    Manual,
  };

  MazeSolver(RobotHardware &hardware, DriveBase &drive);

  void begin();
  void update();

  void setMode(Mode mode);
  Mode mode() const;
  bool isCompleted() const;

private:
  void basicMovement(uint8_t front, double rightDistanceCm, bool rightBlocked);
  void islandMovement(uint8_t front, double rightDistanceCm, bool rightBlocked);
  void applyWallFollowAdjustment(double rightDistanceCm);
  void applyIslandEntryStabilization(double rightDistanceCm);
  bool shouldAggressiveIslandRecentering(double rightDistanceCm) const;
  void markCompleted();

  RobotHardware &hardware;
  DriveBase &drive;
  Mode mode_;
  bool insideMode_;
  bool turned_;
  int islandEntryChecksRemaining_;
  bool completed_;
  int consecutiveLeftTurns_;
};

} // namespace maze