#pragma once

#include <memory>
#include <sstream>
#include <string>

#include "Error.hpp"

namespace vrml_proc::core::error {
  class JsonError : public Error {
   public:
    JsonError(const std::string& details) : m_details(details) {}

   protected:
    std::string GetMessageInternal() const override {
      std::ostringstream stream;
      stream << "[JsonError]: while manipulating with JSON object, an error occured. More details: <" << m_details
             << ">.\n";
      return stream.str();
    }

   private:
    std::string m_details;
  };
}  // namespace vrml_proc::core::error
