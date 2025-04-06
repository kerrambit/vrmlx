#pragma once

#include <iostream>
#include <vector>

#include "Vec2f.hpp"
#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
  namespace parser {
    struct VRMLPROCESSING_API Vec2fArray {
      Vec2fArray() {}

      std::vector<Vec2f> vectors;

      void Add(const Vec2f& vector) { vectors.push_back(vector); }
    };
  }  // namespace parser
}  // namespace vrml_proc
