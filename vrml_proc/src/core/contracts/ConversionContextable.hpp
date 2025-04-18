#pragma once

#include <type_traits>

#include "BaseConversionContext.hpp"

/**
 * @brief ConversionContextable are types that are inherited from BaseConversionContext class.
 */
template <typename T>
concept ConversionContextable = std::is_base_of_v<vrml_proc::conversion_context::BaseConversionContext, T>;
