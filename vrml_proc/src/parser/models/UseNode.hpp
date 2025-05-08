#pragma once

#include <string>

namespace vrml_proc::parser {
  /**
   * @brief Represents a VRML 2.0 USE node.
   */
  struct UseNode {
    UseNode() : identifier("") {}
    std::string identifier;
  };
}  // namespace vrml_proc::parser
