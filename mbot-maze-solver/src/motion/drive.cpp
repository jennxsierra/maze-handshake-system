#include "motion/drive.h"

#include "config/config.h"

namespace maze {

DriveBase::DriveBase(RobotHardware &hardware) : hardware(hardware) {}

// Stops all motor movement.
void DriveBase::stop() {
  hardware.stopMotors();
}

// Moves forward with right motor slightly faster to balance chassis.
void DriveBase::moveForward() {
  Serial.println("[MOTOR] Forward");
  hardware.runLeftMotor(config::kBaseSpeed + 20);
  hardware.runRightMotor(-config::kBaseSpeed - 20);
}

// Moves backward at base speed.
void DriveBase::moveBackward() {
  Serial.println("[MOTOR] Backward");
  hardware.runLeftMotor(-config::kBaseSpeed);
  hardware.runRightMotor(config::kBaseSpeed);
}

// Performs 90° left turn.
void DriveBase::turnLeft() {
  Serial.println("[MOTOR] TURN LEFT");
  runTurnSequence(config::kPauseTurnMs, config::kPauseSpinMs);
}

// Shifts right while maintaining forward motion.
void DriveBase::moveRight() {
  Serial.println("[MOTOR] MOVE RIGHT");
  hardware.runLeftMotor(config::kBaseSpeed * 3);
  hardware.runRightMotor(-config::kBaseSpeed);
}

// Performs 90° right turn (mirrored left turn).
void DriveBase::turnRight() {
  Serial.println("[MOTOR] TURN RIGHT");
  stop();
  delay(config::kPauseShortMs);

  // Turn right.
  hardware.runLeftMotor(config::kBaseSpeed);
  hardware.runRightMotor(-config::kBaseSpeed);
  delay(config::kPauseTurnMs);

  stop();
  delay(config::kPauseShortMs);

  // Spin to complete 90°.
  hardware.runLeftMotor(config::kBaseSpeed * 3.5);
  hardware.runRightMotor(config::kBaseSpeed * 3.5);
  delay(config::kPauseSpinMs);

  stop();
  delay(config::kPauseLongMs);
}

// Performs 180° turnaround with wall-following readjustment for island transitions.
void DriveBase::turnAround() {
  Serial.println("[MOTOR] TURN AROUND");
  // Complete 180° rotation.
  runTurnSequence(config::kPauseTurnAroundTurnMs, config::kPauseTurnAroundSpinMs);

  // Readjust strongly to wall before moving forward.
  readjustRightwardsVeryStrong();
  delay(config::kPauseIslandEntryMs);
  moveForward();
  delay(config::kPauseMediumMs);
}

// Core turning mechanism: turns at turnDelayMs then spins at spinDelayMs for smooth 90° rotation.
void DriveBase::runTurnSequence(unsigned long turnDelayMs, unsigned long spinDelayMs) {
  stop();
  delay(config::kPauseShortMs);

  // Initial turn motion.
  hardware.runLeftMotor(-config::kBaseSpeed);
  hardware.runRightMotor(config::kBaseSpeed);
  delay(turnDelayMs);

  stop();
  delay(config::kPauseShortMs);

  // Spin to complete turn.
  hardware.runLeftMotor(-config::kBaseSpeed * 3.5);
  hardware.runRightMotor(-config::kBaseSpeed * 3.5);
  delay(spinDelayMs);

  stop();
  delay(config::kPauseLongMs);
}

// Light leftward adjustment while maintaining forward momentum.
void DriveBase::readjustLeftwards() {
  Serial.println("[MOTOR] READJUST LEFT");
  hardware.runLeftMotor(config::kBaseSpeed);
  hardware.runRightMotor(-config::kBaseSpeed - 20);
}

// Strong leftward adjustment.
void DriveBase::readjustLeftwardsStrong() {
  Serial.println("[MOTOR] READJUST LEFT STRONG");
  hardware.runLeftMotor(config::kBaseSpeed);
  hardware.runRightMotor(-config::kBaseSpeed - 40);
}

// Light rightward adjustment (wall-following).
void DriveBase::readjustRightwards() {
  Serial.println("[MOTOR] READJUST RIGHT");
  hardware.runLeftMotor(config::kBaseSpeed + 22);
  hardware.runRightMotor(-config::kBaseSpeed);
}

// Strong rightward adjustment.
void DriveBase::readjustRightwardsStrong() {
  Serial.println("[MOTOR] READJUST RIGHT STRONG");
  hardware.runLeftMotor(config::kBaseSpeed + 35);
  hardware.runRightMotor(-config::kBaseSpeed);
}

// Very strong rightward adjustment (for island entry/recovery).
void DriveBase::readjustRightwardsVeryStrong() {
  Serial.println("[MOTOR] READJUST RIGHT VERY STRONG");
  hardware.runLeftMotor(config::kBaseSpeed + 48);
  hardware.runRightMotor(-config::kBaseSpeed);
}

}
