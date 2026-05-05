#pragma once

#include <Arduino.h>

#include "hardware/hardware.h"

namespace maze {

// Handles autonomous maze-solving movements (forward, turning, wall-following adjustments).
class DriveBase {
public:
  explicit DriveBase(RobotHardware &hardware);

  void stop();
  void moveForward();
  void moveBackward();
  void turnLeft();
  void moveRight();
  void turnRight();
  void turnAround();
  void readjustLeftwards();
  void readjustLeftwardsStrong();
  void readjustRightwards();
  void readjustRightwardsStrong();
  void readjustRightwardsVeryStrong();

private:
  void runTurnSequence(unsigned long turnDelayMs, unsigned long spinDelayMs);

  RobotHardware &hardware;
};

}
