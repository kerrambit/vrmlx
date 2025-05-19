#pragma once

#include <functional>

#include "CalculatorResult.hpp"
#include "Int32Array.hpp"
#include "TransformationMatrix.hpp"
#include "Vec3fArray.hpp"

#include "ToGeomExport.hpp"

namespace to_geom::calculator {
  /**
   * @brief Represents a calculator for generating 3D meshes from IndexedFaceSet VRML node.
   */
  class TOGEOM_API IndexedFaceSetCalculator {
   public:
    /**
     * @brief Generates 3D mesh from coordinates and coordinates indices lists.
     *
     * @param coordinateIndices list of coordinate indices (tuple of n numbers seperated by -1 form a face)
     * @param coordinates points (aka coordinates) in the space
     * @param matrix transformation matrix applied to points
     * @param checkRange flag indicating if coordinate index should be checked for range when accessing `coordinates`
     * list
     * @note You may want to set `checkRange` to false, if you know the indicis in your file are valid, as the
     * calculations are (slightly) optimzed for this case.
     * @returns calculator result (mesh object or error if generation failed)
     */
    to_geom::calculator::CalculatorResult Generate3DMesh(
        std::reference_wrapper<const vrml_proc::parser::model::Int32Array> coordinateIndices,
        std::reference_wrapper<const vrml_proc::parser::model::Vec3fArray> coordinates,
        const vrml_proc::math::TransformationMatrix& matrix,
        bool checkRange = true);
  };
}  // namespace to_geom::calculator
