#include "maze/hardware.h"

#include <MeMegaPi.h>

#include "maze/config.h"

namespace maze {

struct RobotHardware::Impl {
  Impl()
      : leftMotor(PORT3B),
        rightMotor(PORT2B),
        rightUltrasonic(PORT_7),
        frontLine(PORT_8),
        rgbLed(PORT_6) {}

  MeMegaPiDCMotor leftMotor;
  MeMegaPiDCMotor rightMotor;
  MeUltrasonicSensor rightUltrasonic;
  MeLineFollower frontLine;
  MeRGBLed rgbLed;
};

RobotHardware::RobotHardware()
    : impl(new Impl()) {}

RobotHardware::~RobotHardware() {
  delete impl;
}

void RobotHardware::begin() {
  clearStatusColor();
  stopMotors();
}

void RobotHardware::stopMotors() {
  impl->leftMotor.run(0);
  impl->rightMotor.run(0);
}

void RobotHardware::runLeftMotor(int16_t speed) {
  impl->leftMotor.run(speed);
}

void RobotHardware::runRightMotor(int16_t speed) {
  impl->rightMotor.run(speed);
}

uint8_t RobotHardware::readFrontLine() {
  return impl->frontLine.readSensors();
}

double RobotHardware::readRightDistanceCm() {
  return impl->rightUltrasonic.distanceCm();
}

bool RobotHardware::isRightBlocked(double rightDistanceCm) const {
  return rightDistanceCm > config::kRightBlockedMinCm &&
         rightDistanceCm < config::kRightBlockedMaxCm;
}

void RobotHardware::setStatusColor(uint8_t red, uint8_t green, uint8_t blue) {
  impl->rgbLed.setColor(0, red, green, blue);
  impl->rgbLed.show();
}

void RobotHardware::clearStatusColor() {
  setStatusColor(0, 0, 0);
}

} // namespace maze