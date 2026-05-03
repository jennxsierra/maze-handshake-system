#pragma once

#include <Arduino.h>

#include "maze/solver.h"

namespace maze {

class SerialCommandRouter {
public:
  explicit SerialCommandRouter(MazeSolver &solver);

  void update();

private:
  MazeSolver &solver;
};

} // namespace maze