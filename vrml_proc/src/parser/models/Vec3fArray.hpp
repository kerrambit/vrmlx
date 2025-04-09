#pragma once

#include <vector>

#include "Vec3f.hpp"

namespace vrml_proc::parser {
  struct Vec3fArray {
    Vec3fArray() : vectors({}) {}

    std::vector<Vec3f> vectors;
  };
}  // namespace vrml_proc::parser
