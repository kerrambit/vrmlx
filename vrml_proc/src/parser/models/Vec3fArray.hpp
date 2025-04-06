#pragma once

#include <vector>
#include <iostream>

#include "Vec3f.hpp"
#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
  namespace parser {
    struct VRMLPROCESSING_API Vec3fArray {
      Vec3fArray() {}

      std::vector<Vec3f> vectors;

      void Add(const Vec3f& vector) { vectors.push_back(vector); }
    };
  }  // namespace parser
}  // namespace vrml_proc
