// Platform-specific hardware implementation using MeMegaPi peripherals.
#include "hardware/hardware.h"

#include <MeMegaPi.h>

#include "config/config.h"

namespace maze {

// Stores actual hardware objects and pin mappings
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

// Initialize hardware: clear status LED and ensure motors are stopped.
void RobotHardware::begin() {
  // Turn off any status LED to start in a known state.
  clearStatusColor();
  // Ensure motors aren't running from previous state.
  stopMotors();
}

// Stop both drive motors immediately.
void RobotHardware::stopMotors() {
  impl->leftMotor.run(0);
  impl->rightMotor.run(0);
}

// Set left motor speed. Positive/negative controls direction.
void RobotHardware::runLeftMotor(int16_t speed) {
  impl->leftMotor.run(speed);
}

// Set right motor speed. Positive/negative controls direction.
void RobotHardware::runRightMotor(int16_t speed) {
  impl->rightMotor.run(speed);
}

// Read the front line follower sensor bitmask.
uint8_t RobotHardware::readFrontLine() {
  return impl->frontLine.readSensors();
}

// Measure distance on the right ultrasonic sensor in centimeters.
double RobotHardware::readRightDistanceCm() {
  return impl->rightUltrasonic.distanceCm();
}

// Determine if the right side is considered blocked by distance thresholds.
bool RobotHardware::isRightBlocked(double rightDistanceCm) const {
  return rightDistanceCm > config::kRightBlockedMinCm &&
         rightDistanceCm < config::kRightBlockedMaxCm;
}

// Set RGB status LED color and show immediately.
void RobotHardware::setStatusColor(uint8_t red, uint8_t green, uint8_t blue) {
  impl->rgbLed.setColor(0, red, green, blue);
  impl->rgbLed.show();
}

// Clear status LED (turn off).
void RobotHardware::clearStatusColor() {
  setStatusColor(0, 0, 0);
}

}
