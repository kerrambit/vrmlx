#pragma once

#include <memory>
#include <string>

#include "Error.hpp"

namespace vrml_proc::parser::error {
  /**
   * @brief Represents a structure describing the error while parsing coming from any grammar.
   *
   * @note This struct is not used, as the mechanism to indentify parser errors in grammars is not implemented yet.
   */
  struct ParserErrorData {
    size_t bytesCountFromStringStartToErrorPosition;
    size_t bytesCountFromErrorPositionToStringEnd;
    std::string expectedRule;
    std::string errorPosition;
  };

  /**
   * @brief Represents an error coming from any grammar.
   *
   * @note This struct is not used, as the mechanism to indentify parser errors in grammars is not implememented yet.
   */
  class GrammarError : public vrml_proc::core::error::Error {
   protected:
    virtual std::string GetMessageInternal() const {
      return "[GrammarError]: while parsing input, grammar <NAME> identified an error.\n";
    }
  };
}  // namespace vrml_proc::parser::error
