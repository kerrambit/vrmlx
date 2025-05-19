#pragma once

#include <vector>

#include "Vec3f.hpp"

namespace vrml_proc::parser::model {
  /**
   * @brief Represents a MF value containing list of vec3f.
   */
  struct Vec3fArray {
    Vec3fArray() : vectors({}) {}

    std::vector<Vec3f> vectors;
  };
}  // namespace vrml_proc::parser::model
