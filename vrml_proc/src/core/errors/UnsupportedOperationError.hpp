#pragma once

#include <memory>
#include <sstream>
#include <string>

#include "Error.hpp"

namespace vrml_proc::core::error {
  class UnsupportedOperationError : public Error {
   public:
    UnsupportedOperationError(std::string message) : m_message(message) {}

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
