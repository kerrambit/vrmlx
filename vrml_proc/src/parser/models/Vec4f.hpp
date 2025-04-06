#pragma once

#include <iostream>

#include "VrmlUnits.hpp"
#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
  namespace parser {
    struct VRMLPROCESSING_API Vec4f {
      Vec4f() {}

      Vec4f(float32_t x, float32_t y, float32_t z, float32_t w) : x(x), y(y), z(z), w(w) {}

      float32_t x = 0.0f;
      float32_t y = 0.0f;
      float32_t z = 0.0f;
      float32_t w = 0.0f;
    };
  }  // namespace parser
}  // namespace vrml_proc
