#pragma once

#include <functional>

#include "Vec3fArray.hpp"
#include "Int32Array.hpp"
#include "TransformationMatrix.hpp"
#include "CalculatorResult.hpp"

#include "ToGeomExport.hpp"

namespace to_geom::calculator {
  class TOGEOM_API IndexedLineSetCalculator {
   public:
    to_geom::calculator::CalculatorResult Generate3DMesh(
        std::reference_wrapper<const vrml_proc::parser::Int32Array> coordinateIndices,
        std::reference_wrapper<const vrml_proc::parser::Vec3fArray> coordinates,
        const vrml_proc::math::TransformationMatrix& matrix);
  };
}  // namespace to_geom::calculator
