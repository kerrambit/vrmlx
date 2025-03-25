#pragma once

#include <functional>

#include "Vec3fArray.hpp"
#include "VectorConversionContext.hpp"

namespace to_geom::conversion_context {
  /**
   * @brief Represents a conversion context which stores a collection of Vec3fArray const references.
   */
  using Vec3fArrayConversionContext = vrml_proc::conversion_context::VectorConversionContext<
      std::reference_wrapper<const vrml_proc::parser::Vec3fArray>>;
}  // namespace to_geom::conversion_context
