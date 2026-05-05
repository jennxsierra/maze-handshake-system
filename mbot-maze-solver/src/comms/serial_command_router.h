#pragma once

#include <Arduino.h>

#include "comms/command_parser.h"

namespace maze {

class SerialCommandRouter {
public:
  explicit SerialCommandRouter(CommandParser &parser);

  void update();

private:
  CommandParser &parser;
};

} // namespace maze