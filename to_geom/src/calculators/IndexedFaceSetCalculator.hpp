#pragma once

#include <functional>

#include "CalculatorResult.hpp"
#include "TransformationMatrix.hpp"
#include "Int32Array.hpp"
#include "Vec3fArray.hpp"

#include "VrmlToGeomExport.hpp"

namespace to_geom::calculator {
  class VRMLTOGEOM_API IndexedFaceSetCalculator {
   public:
    to_geom::calculator::CalculatorResult Generate3DMesh(
        std::reference_wrapper<const vrml_proc::parser::Int32Array> coordinateIndices,
        std::reference_wrapper<const vrml_proc::parser::Vec3fArray> coordinates,
        std::reference_wrapper<const bool> isConvex,
        const vrml_proc::math::TransformationMatrix& matrix);
  };
}  // namespace to_geom::calculator
