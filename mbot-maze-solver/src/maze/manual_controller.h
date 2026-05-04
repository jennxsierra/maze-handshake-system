#pragma once

#include <Arduino.h>

#include "maze/hardware.h"
#include "maze/config.h"

namespace maze {

class ManualController {
public:
  explicit ManualController(RobotHardware &hardware);

  void stop();
  void driveForward(int16_t speed);
  void driveBackward(int16_t speed);
  void pivotLeft(int16_t speed);
  void pivotRight(int16_t speed);
  void pivotLeft90();
  void pivotRight90();

private:
  void drive(int16_t leftSpeed, int16_t rightSpeed);

  RobotHardware &hardware;
};

} // namespace maze
