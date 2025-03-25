#pragma once

#include <functional>

#include "CalculatorResult.hpp"

namespace to_geom::core {
  /**
   * @brief Represents a task (function) which returns CalculatorResult.
   */
  using MeshTask = std::function<to_geom::calculator::CalculatorResult()>;
}  // namespace to_geom::core
