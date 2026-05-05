#include "comms/serial_command_router.h"

namespace maze
{
  // Construct with the central `CommandParser` to forward parsed chars to.
  SerialCommandRouter::SerialCommandRouter(CommandParser &parser) : parser(parser) {}

  // Read all available bytes from USB Serial and forward non-CR/LF chars.
  void SerialCommandRouter::update()
  {
    while (Serial.available() > 0)
    {
      // Read a single byte and ignore newline characters.
      const char command = static_cast<char>(Serial.read());

      if (command == '\n' || command == '\r')
      {
        continue;
      }

      // Forward character to the shared command parser.
      parser.parseCommand(command);
    }
  }
}
