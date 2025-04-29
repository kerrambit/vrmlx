#pragma once

#include <memory>
#include <sstream>
#include <string>

namespace vrml_proc::core::error {
  class NullPointerError : public Error {
   protected:
    std::string GetMessageInternal() const override {
      return "[NullPointerError]: (smart) pointer contained unexpectedly null pointer!\n";
    }
  };
}  // namespace vrml_proc::core::error
