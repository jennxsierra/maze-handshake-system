#pragma once

#include <Arduino.h>

#include "maze/hardware.h"

namespace maze {

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

} // namespace maze