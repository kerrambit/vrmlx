#pragma once

#include <memory>

#include <result.hpp>

#include "Error.hpp"
#include "Mesh.hpp"
#include "Vec3fArray.hpp"
#include "Int32Array.hpp"
#include "TransformationMatrix.hpp"

#include "VrmlToGeomExport.hpp"

namespace to_geom::calculator {
  class VRMLTOGEOM_API IndexedLineSetCalculator {
   public:
    cpp::result<std::shared_ptr<core::Mesh>, std::shared_ptr<vrml_proc::core::error::Error>> Generate3DMesh(
        std::reference_wrapper<const vrml_proc::parser::Int32Array> coordinateIndices,
        std::reference_wrapper<const vrml_proc::parser::Vec3fArray> coordinates,
        const vrml_proc::math::TransformationMatrix& matrix);
  };
}  // namespace to_geom::calculator
