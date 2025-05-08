#pragma once

#include <vector>

#include "VrmlNode.hpp"

namespace vrml_proc::parser {
  /**
   * @brief Represents a VRML file: list of any number of root nodes.
   */
  using VrmlFile = std::vector<VrmlNode>;
}  // namespace vrml_proc::parser
