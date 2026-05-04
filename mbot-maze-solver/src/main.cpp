#include <Arduino.h>

#include "maze/ble_command_router.h"
#include "maze/command_parser.h"
#include "maze/drive.h"
#include "maze/hardware.h"
#include "maze/serial_command_router.h"
#include "maze/solver.h"

namespace {

maze::RobotHardware hardware;
maze::DriveBase drive(hardware);
maze::MazeSolver solver(hardware, drive);
maze::ManualController motorController(hardware);
maze::CommandParser commandParser(solver, hardware, drive, motorController);

// Keep serial router for USB debugging / manual testing.
maze::SerialCommandRouter serialCommandRouter(commandParser);

// BLE router for BLEV1.0_Z module.
maze::BleCommandRouter bleCommandRouter(commandParser, Serial3);

constexpr unsigned long kUsbSerialBaud = 115200;
constexpr unsigned long kBleSerialBaud = 115200;

} // namespace

void setup() {
  Serial.begin(kUsbSerialBaud);
  Serial.println("===== SYSTEM START =====");

  hardware.begin();
  solver.begin();

  // Start BLE UART.
  bleCommandRouter.begin(kBleSerialBaud);

  Serial.println("[BLE] Listening for commands on Serial1...");
}

void loop() {
  // Allow commands from BOTH sources:
  // - BLE (gamepad/keyboard via mbot-controller -> BLE)
  // - USB Serial (debugging)
  bleCommandRouter.update();
  serialCommandRouter.update();

  // Autonomous movement runs only if solver is not in Manual mode.
  solver.update();
}