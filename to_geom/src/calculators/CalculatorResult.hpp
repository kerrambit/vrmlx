#pragma once

#include <memory>

#include <result.hpp>

#include "Error.hpp"
#include "Mesh.hpp"

namespace to_geom::calculator {
  /**
   * @brief Represents a result type. On failure, Error is returned. Otherwise, a mesh is returned.
   */
  using CalculatorResult =
      cpp::result<std::shared_ptr<to_geom::core::Mesh>, std::shared_ptr<vrml_proc::core::error::Error>>;
}  // namespace to_geom::calculator
