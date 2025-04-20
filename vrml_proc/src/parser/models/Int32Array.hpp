#pragma once

#include <vector>
#include <cstdint>

namespace vrml_proc::parser {
  struct Int32Array {
    Int32Array() : integers({}) {}

    std::vector<int32_t> integers;
  };
}  // namespace vrml_proc::parser
