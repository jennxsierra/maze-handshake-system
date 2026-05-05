#include "motion/manual_controller.h"

namespace maze
{
  ManualController::ManualController(RobotHardware &hardware)
      : hardware(hardware) {}

  // Sets left and right motor speeds directly.
  void ManualController::drive(int16_t leftSpeed, int16_t rightSpeed)
  {
    hardware.runLeftMotor(leftSpeed);
    hardware.runRightMotor(rightSpeed);
  }

  // Stops all motors.
  void ManualController::stop() { hardware.stopMotors(); }

  // Drives forward at specified speed.
  void ManualController::driveForward(int16_t speed)
  {
    drive(speed, -speed);
  }

  // Drives backward at specified speed.
  void ManualController::driveBackward(int16_t speed)
  {
    drive(-speed, speed);
  }

  // Pivots left (both motors backward).
  void ManualController::pivotLeft(int16_t speed)
  {
    drive(-speed, -speed);
  }

  // Pivots right (both motors forward).
  void ManualController::pivotRight(int16_t speed)
  {
    drive(speed, speed);
  }

  // Executes exact 90° left pivot.
  void ManualController::pivotLeft90()
  {
    stop();
    delay(config::kPauseShort);

    // Turn left.
    hardware.runLeftMotor(-config::kSpeed);
    hardware.runRightMotor(config::kSpeed);
    delay(config::kPauseTurn);

    stop();
    delay(config::kPauseShort);

    // Spin to complete 90°.
    hardware.runLeftMotor(-config::kSpeed * 3.6);
    hardware.runRightMotor(-config::kSpeed * 3.6);
    delay(config::kPauseSpin);

    stop();
    delay(config::kPauseLong);
  }

  // Executes exact 90° right pivot.
  void ManualController::pivotRight90()
  {
    stop();
    delay(config::kPauseShort);

    // Turn right.
    hardware.runLeftMotor(config::kSpeed);
    hardware.runRightMotor(-config::kSpeed);
    delay(config::kPauseTurn);

    stop();
    delay(config::kPauseShort);

    // Spin to complete 90°.
    hardware.runLeftMotor(config::kSpeed * 3.5);
    hardware.runRightMotor(config::kSpeed * 3.5);
    delay(config::kPauseSpin);

    stop();
    delay(config::kPauseLong);
  }
}
