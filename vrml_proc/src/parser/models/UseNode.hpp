#pragma once

#include <string>
#include <iostream>

#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
  namespace parser {
    struct VRMLPROCESSING_API UseNode {
      UseNode() {}
      std::string identifier;
    };
  }  // namespace parser
}  // namespace vrml_proc
