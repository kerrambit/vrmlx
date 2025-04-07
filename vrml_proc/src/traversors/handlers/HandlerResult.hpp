#pragma once

#include <memory>

#include <result.hpp>

#include "Error.hpp"

namespace vrml_proc::traversor::handler {
  /**
   * @brief Represents a result of any handler.
   */
  template <typename ConversionContext>
  using HandlerResult = cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>>;
}  // namespace vrml_proc::traversor::handler
