#pragma once

#include <string>

namespace vrml_proc::parser {
  struct UseNode {
    UseNode() : identifier("") {}
    std::string identifier;
  };
}  // namespace vrml_proc::parser
