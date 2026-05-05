#include "comms/serial_command_router.h"

namespace maze {

SerialCommandRouter::SerialCommandRouter(CommandParser &parser) : parser(parser) {}

void SerialCommandRouter::update() {
  while (Serial.available() > 0) {
    const char command = static_cast<char>(Serial.read());

    if (command == '\n' || command == '\r') {
      continue;
    }

    parser.parseCommand(command);
  }
}

} // namespace maze