#include "motion/manual_controller.h"

namespace maze {

ManualController::ManualController(RobotHardware &hardware)
    : hardware(hardware) {}

// Sets left and right motor speeds directly.
void ManualController::drive(int16_t leftSpeed, int16_t rightSpeed) {
  hardware.runLeftMotor(leftSpeed);
  hardware.runRightMotor(rightSpeed);
}

// Stops all motors.
void ManualController::stop() { hardware.stopMotors(); }

// Drives forward at specified speed.
void ManualController::driveForward(int16_t speed) {
  drive(speed, -speed);
}

// Drives backward at specified speed.
void ManualController::driveBackward(int16_t speed) {
  drive(-speed, speed);
}

// Pivots left (both motors backward).
void ManualController::pivotLeft(int16_t speed) {
  drive(-speed, -speed);
}

// Pivots right (both motors forward).
void ManualController::pivotRight(int16_t speed) {
  drive(speed, speed);
}

// Executes exact 90° left pivot.
void ManualController::pivotLeft90() {
  stop();
  delay(config::kPauseShortMs);

  // Turn left.
  hardware.runLeftMotor(-config::kBaseSpeed);
  hardware.runRightMotor(config::kBaseSpeed);
  delay(config::kPauseTurnMs);

  stop();
  delay(config::kPauseShortMs);

  // Spin to complete 90°.
  hardware.runLeftMotor(-config::kBaseSpeed * 3.6);
  hardware.runRightMotor(-config::kBaseSpeed * 3.6);
  delay(config::kPauseSpinMs);

  stop();
  delay(config::kPauseLongMs);
}

// Executes exact 90° right pivot.
void ManualController::pivotRight90() {
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

}
