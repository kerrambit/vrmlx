#pragma once

#include <memory>

#include <result.hpp>

#include "Error.hpp"
#include "ConversionContextable.hpp"

namespace vrml_proc::traversor {
  /**
   * @brief Represents a result of a traversal.
   */
  template <ConversionContextable ConversionContext>
  using TraversorResult =
      cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>>;
}  // namespace vrml_proc::traversor
