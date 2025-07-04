#pragma once

#include <type_traits>

#include "BaseConversionContext.hpp"

namespace vrml_proc::core::contract {
  /**
   * @brief ConversionContextable are types that are inherited from BaseConversionContext class.
   * @tparam T type to be tested for comparability
   */
  template <typename T>
  concept ConversionContextable = std::is_base_of_v<vrml_proc::conversion_context::BaseConversionContext, T>;
}  // namespace vrml_proc::core::contract
