#include "comms/ble_command_router.h"

namespace maze {

BleCommandRouter::BleCommandRouter(CommandParser &parser, HardwareSerial &rxSerial)
  : parser(parser), rxSerial(rxSerial) {}

void BleCommandRouter::begin(unsigned long baud) {
  rxSerial.begin(baud);
}

// Reads ALL available BLE bytes and forwards them as commands.
// Filters out CR/LF so you can still use terminal-like senders if needed.
void BleCommandRouter::update() {
  while (rxSerial.available() > 0) {
    const char command = static_cast<char>(rxSerial.read());

    if (command == '\n' || command == '\r') {
      continue;
    }

    parser.parseCommand(command);
  }
}

} // namespace maze
