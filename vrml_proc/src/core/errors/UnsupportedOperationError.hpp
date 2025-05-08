#pragma once

#include <memory>
#include <sstream>
#include <string>

#include "Error.hpp"

namespace vrml_proc::core::error {
  /**
   * @brief Represents an error for an operation, which is not supported.
   *
   * The typical usage can be to return this error for some action, which is not yet supported, but might be in the
   * future.
   */
  class UnsupportedOperationError : public Error {
   public:
    UnsupportedOperationError(const std::string& message) : m_message(message) {}

   protected:
    std::string GetMessageInternal() const override {
      std::ostringstream stream;
      stream << "[UnsupportedOperationError]:" << m_message << "\n";
      return stream.str();
    }

   private:
    std::string m_message;
  };
}  // namespace vrml_proc::core::error
