#include "maze/drive.h"

#include "maze/config.h"

namespace maze {

DriveBase::DriveBase(RobotHardware &hardware) : hardware(hardware) {}

void DriveBase::stop() {
  hardware.stopMotors();
}

void DriveBase::moveForward() {
  Serial.println("[MOTOR] Forward");
  hardware.runLeftMotor(config::kBaseSpeed + 20);
  hardware.runRightMotor(-config::kBaseSpeed - 20);
}

void DriveBase::moveBackward() {
  Serial.println("[MOTOR] Backward");
  hardware.runLeftMotor(-config::kBaseSpeed);
  hardware.runRightMotor(config::kBaseSpeed);
}

void DriveBase::turnLeft() {
  Serial.println("[MOTOR] TURN LEFT");
  stop();
  delay(config::kPauseShortMs);

  hardware.runLeftMotor(-config::kBaseSpeed);
  hardware.runRightMotor(config::kBaseSpeed);
  delay(config::kPauseTurnMs);

  stop();
  delay(config::kPauseShortMs);

  hardware.runLeftMotor(-config::kBaseSpeed * 4);
  hardware.runRightMotor(-config::kBaseSpeed * 4);
  delay(config::kPauseSpinMs);

  stop();
  delay(config::kPauseLongMs);
}

void DriveBase::moveRight() {
  Serial.println("[MOTOR] MOVE RIGHT");
  hardware.runLeftMotor(config::kBaseSpeed * 3);
  hardware.runRightMotor(-config::kBaseSpeed);
}

void DriveBase::turnAround() {
  turnLeft();
  readjustRightwardsVeryStrong();
  delay(config::kPauseIslandEntryMs);
  moveForward();
  delay(config::kPauseMediumMs);
}

void DriveBase::readjustLeftwards() {
  Serial.println("[MOTOR] READJUST LEFT");
  hardware.runLeftMotor(config::kBaseSpeed);
  hardware.runRightMotor(-config::kBaseSpeed - 20);
}

void DriveBase::readjustLeftwardsStrong() {
  Serial.println("[MOTOR] READJUST LEFT STRONG");
  hardware.runLeftMotor(config::kBaseSpeed);
  hardware.runRightMotor(-config::kBaseSpeed - 40);
}

void DriveBase::readjustRightwards() {
  Serial.println("[MOTOR] READJUST RIGHT");
  hardware.runLeftMotor(config::kBaseSpeed + 20);
  hardware.runRightMotor(-config::kBaseSpeed);
}

void DriveBase::readjustRightwardsStrong() {
  Serial.println("[MOTOR] READJUST RIGHT STRONG");
  hardware.runLeftMotor(config::kBaseSpeed + 35);
  hardware.runRightMotor(-config::kBaseSpeed);
}

void DriveBase::readjustRightwardsVeryStrong() {
  Serial.println("[MOTOR] READJUST RIGHT VERY STRONG");
  hardware.runLeftMotor(config::kBaseSpeed + 48);
  hardware.runRightMotor(-config::kBaseSpeed);
}

} // namespace maze