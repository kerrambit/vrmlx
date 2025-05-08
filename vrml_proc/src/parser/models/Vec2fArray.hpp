#pragma once

#include <vector>

#include "Vec2f.hpp"

namespace vrml_proc::parser {
  /**
   * @brief Represents MF value containing list of vec2f.
   */
  struct Vec2fArray {
    Vec2fArray() : vectors({}) {}

    std::vector<Vec2f> vectors;
  };
}  // namespace vrml_proc::parser
