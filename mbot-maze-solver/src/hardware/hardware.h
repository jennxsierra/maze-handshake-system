#pragma once

#include <Arduino.h>

namespace maze {

class RobotHardware {
public:
  RobotHardware();
  ~RobotHardware();

  RobotHardware(const RobotHardware &) = delete;
  RobotHardware &operator=(const RobotHardware &) = delete;

  void begin();

  void stopMotors();
  void runLeftMotor(int16_t speed);
  void runRightMotor(int16_t speed);

  uint8_t readFrontLine();
  double readRightDistanceCm();
  bool isRightBlocked(double rightDistanceCm) const;

  void setStatusColor(uint8_t red, uint8_t green, uint8_t blue);
  void clearStatusColor();

private:
  struct Impl;
  Impl *impl;
};

} // namespace maze