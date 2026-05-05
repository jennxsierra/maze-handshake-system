#pragma once

#include <Arduino.h>

#include "comms/command_parser.h"

namespace maze
{
  // Reads incoming bytes from USB serial and forwards characters to the command parser.
  class SerialCommandRouter
  {
  public:
    explicit SerialCommandRouter(CommandParser &parser);

    void update();

  private:
    CommandParser &parser;
  };
}
