#pragma once

#include <iostream>
#include <vector>
#include <cstdint>

#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
  namespace parser {
    struct VRMLPROCESSING_API Int32Array {
      Int32Array() {}

      std::vector<int32_t> integers;
    };
  }  // namespace parser
}  // namespace vrml_proc
