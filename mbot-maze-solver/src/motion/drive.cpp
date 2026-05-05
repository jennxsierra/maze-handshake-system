#include "motion/drive.h"
#include "config/config.h"

namespace maze
{
  DriveBase::DriveBase(RobotHardware &hardware) : hardware(hardware) {}

  // Stops all motor movement.
  void DriveBase::stop()
  {
    hardware.stopMotors();
  }

  // Moves forward with right motor slightly faster to balance chassis.
  void DriveBase::moveForward()
  {
    Serial.println("[MOTOR] Forward");
    hardware.runLeftMotor(config::kSpeed + 20);
    hardware.runRightMotor(-config::kSpeed - 20);
  }

  // Moves backward at base speed.
  void DriveBase::moveBackward()
  {
    Serial.println("[MOTOR] Backward");
    hardware.runLeftMotor(-config::kSpeed);
    hardware.runRightMotor(config::kSpeed);
  }

  // Performs 90° left turn.
  void DriveBase::turnLeft()
  {
    Serial.println("[MOTOR] TURN LEFT");
    runTurnSequence(config::kPauseTurn, config::kPauseSpin);
  }

  // Shifts right while maintaining forward motion.
  void DriveBase::moveRight()
  {
    Serial.println("[MOTOR] MOVE RIGHT");
    hardware.runLeftMotor(config::kSpeed * 3);
    hardware.runRightMotor(-config::kSpeed);
  }

  // Performs 90° right turn (mirrored left turn).
  void DriveBase::turnRight()
  {
    Serial.println("[MOTOR] TURN RIGHT");
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

  // Performs 180° turnaround with wall-following readjustment for island transitions.
  void DriveBase::turnAround()
  {
    Serial.println("[MOTOR] TURN AROUND");
    // Complete 180° rotation.
    runTurnSequence(config::kPauseTurnAroundTurn, config::kPauseTurnAroundSpin);

    // Readjust strongly to wall before moving forward.
    adjustRightVeryStrong();
    delay(config::kPauseIslandEntry);
    moveForward();
    delay(config::kPauseMedium);
  }

  // Core turning mechanism: turns at turnDelayMs then spins at spinDelayMs for smooth 90° rotation.
  void DriveBase::runTurnSequence(unsigned long turnDelayMs, unsigned long spinDelayMs)
  {
    stop();
    delay(config::kPauseShort);

    // Initial turn motion.
    hardware.runLeftMotor(-config::kSpeed);
    hardware.runRightMotor(config::kSpeed);
    delay(turnDelayMs);

    stop();
    delay(config::kPauseShort);

    // Spin to complete turn.
    hardware.runLeftMotor(-config::kSpeed * 3.5);
    hardware.runRightMotor(-config::kSpeed * 3.5);
    delay(spinDelayMs);

    stop();
    delay(config::kPauseLong);
  }

  // Light leftward adjustment while maintaining forward momentum.
  void DriveBase::adjustLeft()
  {
    Serial.println("[MOTOR] READJUST LEFT");
    hardware.runLeftMotor(config::kSpeed);
    hardware.runRightMotor(-config::kSpeed - 20);
  }

  // Strong leftward adjustment.
  void DriveBase::adjustLeftStrong()
  {
    Serial.println("[MOTOR] READJUST LEFT STRONG");
    hardware.runLeftMotor(config::kSpeed);
    hardware.runRightMotor(-config::kSpeed - 40);
  }

  // Light rightward adjustment (wall-following).
  void DriveBase::adjustRight()
  {
    Serial.println("[MOTOR] READJUST RIGHT");
    hardware.runLeftMotor(config::kSpeed + 22);
    hardware.runRightMotor(-config::kSpeed);
  }

  // Strong rightward adjustment.
  void DriveBase::adjustRightStrong()
  {
    Serial.println("[MOTOR] READJUST RIGHT STRONG");
    hardware.runLeftMotor(config::kSpeed + 35);
    hardware.runRightMotor(-config::kSpeed);
  }

  // Very strong rightward adjustment (for island entry/recovery).
  void DriveBase::adjustRightVeryStrong()
  {
    Serial.println("[MOTOR] READJUST RIGHT VERY STRONG");
    hardware.runLeftMotor(config::kSpeed + 48);
    hardware.runRightMotor(-config::kSpeed);
  }
}
