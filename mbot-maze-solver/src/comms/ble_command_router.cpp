#include "comms/ble_command_router.h"

namespace maze
{
  // Constructor stores references to the parser and the BLE UART instance.
  BleCommandRouter::BleCommandRouter(CommandParser &parser, HardwareSerial &rxSerial)
      : parser(parser), rxSerial(rxSerial) {}

  // Configure BLE UART baud rate.
  void BleCommandRouter::begin(unsigned long baud)
  {
    rxSerial.begin(baud);
  }

  // Read all available BLE bytes and forward them to the parser.
  // CR/LF are ignored to allow terminal-like senders.
  void BleCommandRouter::update()
  {
    while (rxSerial.available() > 0)
    {
      const char command = static_cast<char>(rxSerial.read());

      if (command == '\n' || command == '\r')
      {
        continue;
      }

      parser.parseCommand(command);
    }
  }
}
