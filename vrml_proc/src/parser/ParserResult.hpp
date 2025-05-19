#pragma once

#include <memory>

#include <result.hpp>

#include "Error.hpp"

namespace vrml_proc::parser {
  /**
   * @brief Represents a result type of a parser.
   *
   * @tparam ResultType parser returns data of this type
   */
  template <typename ResultType>
  using ParserResult = cpp::result<ResultType, std::shared_ptr<vrml_proc::core::error::Error>>;
}  // namespace vrml_proc::parser
