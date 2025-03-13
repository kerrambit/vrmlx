#pragma once

#include <CGAL/Kernel/interface_macros.h>
#include <CGAL/Simple_cartesian.h>

#include "Vec3f.hpp"
#include "Vec3fArray.hpp"
#include "VrmlUnits.hpp"

namespace vrml_proc::math::cgal {

  using CGALKernel = CGAL::Simple_cartesian<vrml_proc::parser::float32_t>;

  using CGALVector3 = CGALKernel::Vector_3;

  using CGALPoint = CGALKernel::Point_3;

  inline CGALVector3 Vec3fToCGALVector3(const vrml_proc::parser::Vec3f& vec3) {
    return CGALVector3(vec3.x, vec3.y, vec3.z);
  }

  inline CGALPoint Vec3fToCGALPoint(const vrml_proc::parser::Vec3f& vec3) { return CGALPoint(vec3.x, vec3.y, vec3.z); }

  inline std::vector<CGALPoint> Vec3fArrayToCGALPoints(const vrml_proc::parser::Vec3fArray& vec3Array) {
    std::vector<CGALPoint> cgalPoints;
    cgalPoints.reserve(vec3Array.vectors.size());

    for (const auto& vec3 : vec3Array.vectors) {
      cgalPoints.emplace_back(vec3.x, vec3.y, vec3.z);
    }

    return cgalPoints;
  }

  inline CGALVector3 CGALPointToCGALVector3(const CGALPoint& point) {
    return CGALVector3(point.x(), point.y(), point.z());
  }

  inline CGALPoint CGALVector3ToCGALPoint(const CGALVector3& vector) {
    return CGALPoint(vector.x(), vector.y(), vector.z());
  }
}  // namespace vrml_proc::math::cgal
