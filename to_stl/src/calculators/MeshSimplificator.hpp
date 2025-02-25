#pragma once

#include <CGAL/Surface_mesh_simplification/edge_collapse.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Count_ratio_stop_predicate.h>

#include "FormatString.hpp"
#include "Logger.hpp"
#include "Mesh.hpp"

#include "VrmlToStlExport.hpp"

namespace to_stl::calculator {

	inline VRMLTOSTL_API void SimplifyMesh(to_stl::core::Mesh& mesh, double stopRation) {

        using namespace vrml_proc::core::logger;
        using namespace vmml_proc::core::utils;

        LogDebug(FormatString("Simplify the mesh (stop ratio is set to ", stopRation, " - stop simplifying when reaching ", stopRation, "% of the original number of edges)."), LOGGING_INFO);

        namespace SMS = CGAL::Surface_mesh_simplification;
        SMS::Count_ratio_stop_predicate<to_stl::core::Mesh> stop(stopRation);
        int collapsedEdgesCount = SMS::edge_collapse(mesh, stop);

        LogDebug(FormatString("Simplification finished. ", collapsedEdgesCount, " were collapsed."), LOGGING_INFO);
	}
}
