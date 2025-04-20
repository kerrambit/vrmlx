#pragma once

#include <vector>

#include <CGAL/Alpha_shape_3.h>
#include <CGAL/Delaunay_triangulation_3.h>
#include <CGAL/Alpha_shape_vertex_base_3.h>
#include <CGAL/Alpha_shape_cell_base_3.h>
#include <CGAL/Triangulation_data_structure_3.h>

#include <result.hpp>

#include "CalculatorError.hpp"
#include "CGALBaseTypesForVrml.hpp"
#include "Error.hpp"
#include "FormatString.hpp"
#include "Logger.hpp"
#include "ManualTimer.hpp"
#include "Mesh.hpp"
#include "TransformationMatrix.hpp"
#include "Vec3fArray.hpp"

using K = vrml_proc::math::cgal::CGALKernel;
using Vb = CGAL::Alpha_shape_vertex_base_3<K>;
using Cb = CGAL::Alpha_shape_cell_base_3<K>;
using Tds = CGAL::Triangulation_data_structure_3<Vb, Cb>;
using Delaunay = CGAL::Delaunay_triangulation_3<K, Tds>;
using AlphaShape = CGAL::Alpha_shape_3<Delaunay>;

namespace to_geom::calculator::AlphaShapeCalculator {

  /**
   * @brief Generates shape reconstruction from a dense unorganized set of data points (aka point cloud).
   * @note Source: https://doc.cgal.org/latest/Alpha_shapes_3/index.html.
   *
   * @param pointCloud set of unorganized points
   * @param alphaValue controls the detail level of the reconstruction (smaller values yield finer detail, whereas
   * larger values produce a smoother, more abstract shape)
   * @param matrix transformation matrix
   *
   * @returns reconstrcuted mesh or error object if the point cloud is e.g. empty
   */
  to_geom::calculator::CalculatorResult Generate3DAlphaShapeMeshForPointCloud(
      std::reference_wrapper<const vrml_proc::parser::Vec3fArray> pointCloud,
      double alphaValue,
      const vrml_proc::math::TransformationMatrix& matrix) {  //

    using vrml_proc::parser::model::validator::error::EmptyArrayError;
    using namespace to_geom::calculator::error;
    using vrml_proc::core::utils::FormatString;
    using namespace vrml_proc::core::logger;
    using namespace vrml_proc::math::cgal;

    LogInfo("Generate 3D alpha shape mesh from point cloud using AlphaShapeCalculator.", LOGGING_INFO);

    auto mesh = std::make_shared<core::Mesh>();
    auto error = std::make_shared<error::AlphaShapeCalculatorError>();

    if (pointCloud.get().vectors.empty()) {
      return cpp::fail(
          error << (std::make_shared<PropertiesError>() << std::make_shared<EmptyArrayError>("pointCloud")));
    }

    std::vector<CGALPoint> points = Vec3fArrayToCGALPoints(pointCloud.get());

    auto timer = vrml_proc::core::utils::ManualTimer();
    timer.Start();

    Delaunay dt(points.begin(), points.end());
    AlphaShape alphaShape(dt, alphaValue, AlphaShape::GENERAL);

    for (auto it = alphaShape.alpha_shape_facets_begin(); it != alphaShape.alpha_shape_facets_end(); ++it) {
      if (alphaShape.classify(*it) == AlphaShape::REGULAR) {
        auto triangle = alphaShape.triangle(*it);

        mesh->add_face(mesh->add_vertex(matrix.transform(triangle.vertex(0))),
            mesh->add_vertex(matrix.transform(triangle.vertex(1))),
            mesh->add_vertex(matrix.transform(triangle.vertex(2))));
      }
    }

    double time = timer.End();
    LogInfo(FormatString("Mesh was generated successfully. The generation took ", time, " seconds."), LOGGING_INFO);

    return mesh;
  };
}  // namespace to_geom::calculator::AlphaShapeCalculator
