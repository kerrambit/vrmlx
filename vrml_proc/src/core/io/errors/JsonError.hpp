#pragma once

#include <sstream>
#include <string>

#include "Error.hpp"

namespace vrml_proc::core::io::error {
  /**
   * @brief Represents an error related to JSON manipulation.
   */
  class JsonError : public vrml_proc::core::error::Error {
   public:
    /**
     * @brief Constructs a JsonError with additional detail information.
     *
     * @param details A string describing the specific cause or context of the error.
     */
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
}  // namespace vrml_proc::core::io::error
