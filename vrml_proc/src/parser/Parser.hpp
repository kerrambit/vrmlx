#pragma once

#include "ParserResult.hpp"

namespace vrml_proc::parser {
  /**
   * @brief Represents a generic Parser class.
   *
   * Parser's job is to take data of `SourceType` and return parsed data of `ResultType` using `Parse()` method.
   */
  template <typename SourceType, typename ResultType>
  class Parser {
   public:
    /**
     * @brief Default constructor.
     */
    Parser() = default;
    /**
     * @brief Default destructor.
     */
    virtual ~Parser() = default;
    /**
     * @brief Pure virtual method which aims to parse `source`.
     *
     * @param source data to parse
     * @returns parsed source in the form of `ResultType`, or error if something unexpected occurs
     */
    virtual ParserResult<ResultType> Parse(SourceType source) = 0;
  };
}  // namespace vrml_proc::parser
