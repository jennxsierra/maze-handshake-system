#include <Arduino.h>

#include "maze/drive.h"
#include "maze/hardware.h"
#include "maze/serial_command_router.h"
#include "maze/solver.h"

namespace {

maze::RobotHardware hardware;
maze::DriveBase drive(hardware);
maze::MazeSolver solver(hardware, drive);
maze::SerialCommandRouter commandRouter(solver);

} // namespace

void setup() {
  Serial.begin(115200);
  Serial.println("===== SYSTEM START =====");
  hardware.begin();
  solver.begin();
}

void loop() {
  commandRouter.update();
  solver.update();
}