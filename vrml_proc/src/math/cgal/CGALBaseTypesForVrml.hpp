#pragma once

#include <CGAL/Kernel/interface_macros.h>
#include <CGAL/Simple_cartesian.h>

#include "Vec3f.hpp"
#include "Vec3fArray.hpp"
#include "VrmlUnits.hpp"

/**
 * @brief Namespace containing basic CGAL types.
 *
 * @todo Ideally, vrmlproc should be link CGAl at all, and all these types should be defined in togeom. However, because
 * vrmlproc's TransformationMatrix is dependent on CGAl matrix type, this cannot be done until we use diiferent
 * abstraction for matrix.
 */
namespace vrml_proc::math::cgal {

  using CGALKernel = CGAL::Simple_cartesian<double>;

  using CGALVector3 = CGALKernel::Vector_3;

  using CGALPoint = CGALKernel::Point_3;

  /**
   * @brief Converts Ve3f to CGAL vector.
   *
   * @param vec3 vector to convert
   * @returns CGAl converted vector
   */
  inline CGALVector3 Vec3fToCGALVector3(const vrml_proc::parser::model::Vec3f& vec3) {
    return CGALVector3(vec3.x, vec3.y, vec3.z);
  }

  /**
   * @brief Converts Ve3f to CGAL point.
   *
   * @param vec3 vector to convert
   * @returns CGAl converted point
   */
  inline CGALPoint Vec3fToCGALPoint(const vrml_proc::parser::model::Vec3f& vec3) { return CGALPoint(vec3.x, vec3.y, vec3.z); }

  /**
   * @brief Converts an array of Vec3f vectors into list of CGAL points.
   *
   * @param vec3Array array of vectors to convert
   * @returns list of CGAL points
   */
  inline std::vector<CGALPoint> Vec3fArrayToCGALPoints(const vrml_proc::parser::model::Vec3fArray& vec3Array) {
    std::vector<CGALPoint> cgalPoints;
    cgalPoints.reserve(vec3Array.vectors.size());

    for (const auto& vec3 : vec3Array.vectors) {
      cgalPoints.emplace_back(vec3.x, vec3.y, vec3.z);
    }

    return cgalPoints;
  }
}  // namespace vrml_proc::math::cgal
