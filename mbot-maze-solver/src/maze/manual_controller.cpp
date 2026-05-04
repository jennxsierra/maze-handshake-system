#include "maze/manual_controller.h"

namespace maze {

ManualController::ManualController(RobotHardware &hardware)
    : hardware(hardware) {}

void ManualController::drive(int16_t leftSpeed, int16_t rightSpeed) {
  hardware.runLeftMotor(leftSpeed);
  hardware.runRightMotor(rightSpeed);
}

void ManualController::stop() { hardware.stopMotors(); }

void ManualController::driveForward(int16_t speed) {
  drive(speed, -speed);
}

void ManualController::driveBackward(int16_t speed) {
  drive(-speed, speed);
}

void ManualController::pivotLeft(int16_t speed) {
  drive(-speed, -speed);
}

void ManualController::pivotRight(int16_t speed) {
  drive(speed, speed);
}

void ManualController::pivotLeft90() {
  // Exact 90° left pivot mirroring DriveBase::runTurnSequence
  stop();
  delay(config::kPauseShortMs);

  hardware.runLeftMotor(-config::kBaseSpeed);
  hardware.runRightMotor(config::kBaseSpeed);
  delay(config::kPauseTurnMs);

  stop();
  delay(config::kPauseShortMs);

  hardware.runLeftMotor(-config::kBaseSpeed * 3.6);
  hardware.runRightMotor(-config::kBaseSpeed * 3.6);
  delay(config::kPauseSpinMs);

  stop();
  delay(config::kPauseLongMs);
}

void ManualController::pivotRight90() {
  // Exact 90° right pivot mirroring DriveBase::turnRight
  stop();
  delay(config::kPauseShortMs);

  hardware.runLeftMotor(config::kBaseSpeed);
  hardware.runRightMotor(-config::kBaseSpeed);
  delay(config::kPauseTurnMs);

  stop();
  delay(config::kPauseShortMs);

  hardware.runLeftMotor(config::kBaseSpeed * 3.5);
  hardware.runRightMotor(config::kBaseSpeed * 3.5);
  delay(config::kPauseSpinMs);

  stop();
  delay(config::kPauseLongMs);
}

} // namespace maze
