#pragma once

#include <Arduino.h>

#include "comms/command_parser.h"

namespace maze
{
  // BleCommandRouter reads single-character commands from the Makeblock BLE module
  // and forwards them to the CommandParser.
  class BleCommandRouter
  {
  public:
    BleCommandRouter(CommandParser &parser, HardwareSerial &rxSerial);
    
    void begin(unsigned long baud);
    void update();

  private:
    CommandParser &parser;
    HardwareSerial &rxSerial;
  };
}
