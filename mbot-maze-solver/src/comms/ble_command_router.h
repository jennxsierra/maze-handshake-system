#pragma once

#include <Arduino.h>

#include "comms/command_parser.h"

namespace maze {

// BleCommandRouter reads single-character commands from the Makeblock BLE module
// and forwards them to the CommandParser.
class BleCommandRouter {
public:
  BleCommandRouter(CommandParser &parser, HardwareSerial &rxSerial);

  // begin configures the UART baud rate used by the BLE module.
  void begin(unsigned long baud);

  // update pumps all available bytes from BLE and forwards valid command chars.
  void update();

private:
  CommandParser &parser;
  HardwareSerial &rxSerial;
};

} // namespace maze
