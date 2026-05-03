#include "maze/serial_command_router.h"

namespace maze {

SerialCommandRouter::SerialCommandRouter(MazeSolver &solver) : solver(solver) {}

void SerialCommandRouter::update() {
  while (Serial.available() > 0) {
    const char command = static_cast<char>(Serial.read());

    if (command == '\n' || command == '\r') {
      continue;
    }

    solver.handleCommand(command);
  }
}

} // namespace maze