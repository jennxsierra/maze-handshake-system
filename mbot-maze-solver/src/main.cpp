#include <Arduino.h>

#include "comms/ble_command_router.h"
#include "comms/command_parser.h"
#include "motion/drive.h"
#include "hardware/hardware.h"
#include "comms/serial_command_router.h"
#include "algorithm/solver.h"

// Global robot subsystems and command routers.
// Initialized once on startup and shared across setup/loop.
namespace
{
  maze::RobotHardware hardware;
  maze::DriveBase drive(hardware);
  maze::MazeSolver solver(hardware, drive);
  maze::ManualController motorController(hardware);
  maze::CommandParser commandParser(solver, hardware, drive, motorController);

  // Keep serial router for USB debugging / manual testing.
  maze::SerialCommandRouter serialCommandRouter(commandParser);

  // BLE router for BLEV1.0_Z module (wireless gamepad/keyboard input).
  maze::BleCommandRouter bleCommandRouter(commandParser, Serial3);

  constexpr unsigned long kUsbSerialBaud = 115200;
  constexpr unsigned long kBleSerialBaud = 115200;

}

// Initialize all hardware, solver, and communication subsystems.
void setup()
{
  // Start USB Serial for debugging output.
  Serial.begin(kUsbSerialBaud);
  Serial.println("===== SYSTEM START =====");

  // Initialize robot hardware and autonomous solver.
  hardware.begin();
  solver.begin();

  // Start BLE UART for wireless command input.
  bleCommandRouter.begin(kBleSerialBaud);

  Serial.println("[BLE] Listening for commands on Serial1...");
}

// Main loop: process commands from dual input sources and run autonomous solver.
void loop()
{
  // Process commands from both BLE (gamepad/keyboard) and USB Serial (debugging).
  bleCommandRouter.update();
  serialCommandRouter.update();

  // Run autonomous maze solver (only active if not in Manual mode).
  solver.update();
}
