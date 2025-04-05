#pragma once

#include <functional>

#include "TransformationMatrix.hpp"
#include "CalculatorResult.hpp"
#include "Vec3f.hpp"

#include "VrmlToGeomExport.hpp"

namespace to_geom {
  namespace calculator {
    class VRMLTOGEOM_API BoxCalculator {
     public:
      to_geom::calculator::CalculatorResult Generate3DMesh(std::reference_wrapper<const vrml_proc::parser::Vec3f> size,
          const vrml_proc::math::TransformationMatrix& matrix);
    };
  }  // namespace calculator
}  // namespace to_geom
