#pragma once

#include <CGAL/Surface_mesh_simplification/edge_collapse.h>
#if __has_include(<CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Edge_count_ratio_stop_predicate.h>)
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Edge_count_ratio_stop_predicate.h>
#else
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Count_ratio_stop_predicate.h>
#endif

#include "FormatString.hpp"
#include "Logger.hpp"
#include "ManualTimer.hpp"
#include "Mesh.hpp"
#include "UnitInterval.hpp"

namespace to_geom::calculator::MeshSimplificator {
  /**
   * @brief Simplifies the mesh by reducing the number of edges.
   *
   * @param mesh mesh to simplify
   * @param stopRatio number <0.0, 1.0> indicating when the simplification must finish, meaning that in the end 1.0 -
   * `stopRatio` of the original mesh edges are collapsed
   */
  inline void SimplifyMesh(to_geom::core::Mesh& mesh, vrml_proc::core::utils::UnitInterval stopRatio) {  //

    using namespace vrml_proc::core::logger;
    using namespace vrml_proc::core::utils;

    LogInfo(
        FormatString("Simplify the mesh (stop ratio is set to ", stopRatio.GetValue(), " - stopping simplifying when ",
            100.0 - stopRatio.GetAsPercentage(), "% of the original number of edges are collapsed)."),
        LOGGING_INFO);

    if (!CGAL::is_triangle_mesh(mesh)) {
      LogError("Mesh cannot be simplified, because the mesh is not a pure triangular mesh.", LOGGING_INFO);
      return;
    }

    if (!CGAL::is_valid_polygon_mesh(mesh)) {
      LogError("Mesh cannot be simplified, because the mesh is not a valid polygon mesh.", LOGGING_INFO);
      return;
    }

    ManualTimer timer;
    timer.Start();

    namespace SMS = CGAL::Surface_mesh_simplification;

#if __has_include(<CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Edge_count_ratio_stop_predicate.h>)
    SMS::Edge_count_ratio_stop_predicate<to_geom::core::Mesh> stop(stopRatio.GetValue());
#else
    SMS::Count_ratio_stop_predicate<to_geom::core::Mesh> stop(stopRatio.GetValue());
#endif

    int collapsedEdgesCount = SMS::edge_collapse(mesh, stop);

    double time = timer.End();

    LogInfo(FormatString(
                "Simplification took ", time, " seconds. In total, ", collapsedEdgesCount, " edges were collapsed."),
        LOGGING_INFO);
  }
}  // namespace to_geom::calculator::MeshSimplificator
