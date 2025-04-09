#pragma once

#include "VrmlUnits.hpp"

namespace vrml_proc::parser {
  struct Vec4f {
    Vec4f() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
    Vec4f(float32_t x, float32_t y, float32_t z, float32_t w) : x(x), y(y), z(z), w(w) {}

    float32_t x;
    float32_t y;
    float32_t z;
    float32_t w;
  };
}  // namespace vrml_proc::parser
