#pragma once

#include <CGAL/Kernel/interface_macros.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh/Surface_mesh.h>

#include "CGALBaseTypesForVrml.hpp"

namespace to_geom::core {
  /**
   * @brief Represents a polygonal surface mesh.
   */
  using Mesh = CGAL::Surface_mesh<vrml_proc::math::cgal::CGALPoint>;
}  // namespace to_geom::core
