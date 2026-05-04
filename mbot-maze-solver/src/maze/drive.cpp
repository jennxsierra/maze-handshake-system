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
  runTurnSequence(config::kPauseTurnMs, config::kPauseSpinMs);
}

void DriveBase::moveRight() {
  Serial.println("[MOTOR] MOVE RIGHT");
  hardware.runLeftMotor(config::kBaseSpeed * 3);
  hardware.runRightMotor(-config::kBaseSpeed);
}

void DriveBase::turnRight() {
  Serial.println("[MOTOR] TURN RIGHT");
  // mirror of runTurnSequence but inverted motors
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

void DriveBase::turnAround() {
  Serial.println("[MOTOR] TURN AROUND");
  runTurnSequence(config::kPauseTurnAroundTurnMs, config::kPauseTurnAroundSpinMs);

  readjustRightwardsVeryStrong();
  delay(config::kPauseIslandEntryMs);
  moveForward();
  delay(config::kPauseMediumMs);
}

void DriveBase::runTurnSequence(unsigned long turnDelayMs, unsigned long spinDelayMs) {
  stop();
  delay(config::kPauseShortMs);

  hardware.runLeftMotor(-config::kBaseSpeed);
  hardware.runRightMotor(config::kBaseSpeed);
  delay(turnDelayMs);

  stop();
  delay(config::kPauseShortMs);

  hardware.runLeftMotor(-config::kBaseSpeed * 3.5);
  hardware.runRightMotor(-config::kBaseSpeed * 3.5);
  delay(spinDelayMs);

  stop();
  delay(config::kPauseLongMs);
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
  hardware.runLeftMotor(config::kBaseSpeed + 22);
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