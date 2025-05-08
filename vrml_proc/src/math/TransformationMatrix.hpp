#pragma once

#include <CGAL/Aff_transformation_3.h>
#include <CGAL/Kernel/interface_macros.h>
#include <CGAL/Kernel/Type_equality_wrapper.h>
#include <CGAL/Simple_cartesian.h>

#include "CGALBaseTypesForVrml.hpp"
#include "Transformation.hpp"

#include "VrmlProcExport.hpp"

namespace vrml_proc::math {

  using TransformationMatrix = CGAL::Aff_transformation_3<vrml_proc::math::cgal::CGALKernel>;

  VRMLPROC_API TransformationMatrix UpdateTransformationMatrix(const TransformationMatrix& currentMatrix,
                                                                     const Transformation& transformationData);
}  // namespace vrml_proc::math
