#pragma once

#include <functional>

#include "TransformationMatrix.hpp"
#include "CalculatorResult.hpp"
#include "Vec3f.hpp"

#include "ToGeomExport.hpp"

namespace to_geom::calculator {
  /**
   * @brief Represents a calculator for Box VRML node.
   */
  class TOGEOM_API BoxCalculator {
   public:
    /**
     * @brief Generated 3D mesh for Box geometry primtive.
     *
     * @param size size of the box (x, y, z)
     * @param matrix tranformation matrix applied to geometry
     * @returns calculator result (mesh object or error if generation failed)
     */
    to_geom::calculator::CalculatorResult Generate3DMesh(std::reference_wrapper<const vrml_proc::parser::model::Vec3f> size,
        const vrml_proc::math::TransformationMatrix& matrix);
  };
}  // namespace to_geom::calculator
