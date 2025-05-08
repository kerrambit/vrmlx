#pragma once

#include <CGAL/Aff_transformation_3.h>
#include <CGAL/Kernel/interface_macros.h>
#include <CGAL/Kernel/Type_equality_wrapper.h>
#include <CGAL/Simple_cartesian.h>

#include "CGALBaseTypesForVrml.hpp"
#include "Transformation.hpp"

#include "VrmlProcExport.hpp"

namespace vrml_proc::math {

  /**
   * @brief Represents a transformation matrix in 3D space.
   *
   * @todo Create new matrix abstraction not based on CGAL.
   */
  using TransformationMatrix = CGAL::Aff_transformation_3<vrml_proc::math::cgal::CGALKernel>;

  /**
   * @brief Creates new matrix by updating current matrix by transformation data.
   *
   * @param currentMatrix current matrix
   * @param transformationData data containing information about new transformations to apply
   * @returns updated matrix
   */
  VRMLPROC_API TransformationMatrix UpdateTransformationMatrix(
      const TransformationMatrix& currentMatrix, const Transformation& transformationData);
}  // namespace vrml_proc::math
