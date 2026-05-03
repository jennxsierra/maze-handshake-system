#pragma once

#include <Arduino.h>

#include "maze/drive.h"

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

  void handleCommand(char command);

private:
  void basicMovement(uint8_t front, double rightDistanceCm, bool rightBlocked);
  void islandMovement(uint8_t front, double rightDistanceCm, bool rightBlocked);
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