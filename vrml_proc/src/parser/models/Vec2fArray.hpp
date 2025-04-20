#pragma once

#include <vector>

#include "Vec2f.hpp"

namespace vrml_proc::parser {
  struct Vec2fArray {
    Vec2fArray() : vectors({}) {}

    std::vector<Vec2f> vectors;
  };
}  // namespace vrml_proc::parser
