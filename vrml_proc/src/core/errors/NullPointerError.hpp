#pragma once

#include <memory>
#include <sstream>
#include <string>

namespace vrml_proc::core::error {
  /**
   * @brief Represents an error due to a null (smart) pointer being unexpectedly encountered.
   *
   * This error is thrown or reported when a pointer, expected to hold a valid object,
   * is found to be null.
   */
  class NullPointerError : public Error {
   protected:
    std::string GetMessageInternal() const override {
      return "[NullPointerError]: (smart) pointer contained unexpectedly null pointer!\n";
    }
  };
}  // namespace vrml_proc::core::error
