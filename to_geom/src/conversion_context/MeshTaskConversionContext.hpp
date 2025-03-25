#pragma once

#include "MeshTask.hpp"
#include "VectorConversionContext.hpp"

namespace to_geom::conversion_context {
  /**
   * @brief Represents a conversion context which stores a collection of mesh tasks.
   */
  using MeshTaskConversionContext = vrml_proc::conversion_context::VectorConversionContext<to_geom::core::MeshTask>;
}  // namespace to_geom::conversion_context
