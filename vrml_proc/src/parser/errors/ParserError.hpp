#pragma once

#include <string>

#include "Error.hpp"

namespace vrml_proc::parser::error {

  class ParserError : public vrml_proc::core::error::Error {
   protected:
    virtual std::string GetMessageInternal() const {
      return "[ParserError]: error occured while parsing VRML input. It is possible that VRML input has invalid "
             "format!\n";
    }
  };
}  // namespace vrml_proc::parser::error
