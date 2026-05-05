#pragma once

#include <Arduino.h>

#include "hardware/hardware.h"
#include "config/config.h"

namespace maze {

// Handles manual motor control via gamepad/keyboard input for testing and debugging.
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

}
