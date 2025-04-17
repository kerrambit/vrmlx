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
#include "Vec3fArray.hpp"
#include "Vec3f.hpp"
#include "Logger.hpp"
#include "ManualTimer.hpp"
#include "Mesh.hpp"
#include "ModelValidationError.hpp"
#include "Range.hpp"
#include "UnsupportedOperationError.hpp"
#include "TransformationMatrix.hpp"

using K = vrml_proc::math::cgal::CGALKernel;
using Vb = CGAL::Alpha_shape_vertex_base_3<K>;
using Cb = CGAL::Alpha_shape_cell_base_3<K>;
using Tds = CGAL::Triangulation_data_structure_3<Vb, Cb>;
using Delaunay = CGAL::Delaunay_triangulation_3<K, Tds>;
using AlphaShape = CGAL::Alpha_shape_3<Delaunay>;

namespace to_geom::calculator::AlphaShapeCalculator {

  /**
   * @note Source: https://doc.cgal.org/latest/Alpha_shapes_3/index.html.
   */
  cpp::result<std::shared_ptr<core::Mesh>, std::shared_ptr<vrml_proc::core::error::Error>>
  Generate3DAlphaShapeMeshForPointCloud(std::reference_wrapper<const vrml_proc::parser::Vec3fArray> pointCloud,
      double alphaValue,
      const vrml_proc::math::TransformationMatrix& matrix) {
    vrml_proc::core::logger::LogInfo(
        "Generate 3D alpha shape mesh from point cloud using AlphaShapeCalculator.", LOGGING_INFO);

    auto mesh = std::make_shared<core::Mesh>();
    auto error = std::make_shared<error::AlphaShapeCalculatorError>();

    std::vector<vrml_proc::math::cgal::CGALPoint> points =
        vrml_proc::math::cgal::Vec3fArrayToCGALPoints(pointCloud.get());

    Delaunay dt(points.begin(), points.end());
    AlphaShape alphaShape(dt, alphaValue, AlphaShape::GENERAL);

    for (auto it = alphaShape.alpha_shape_facets_begin(); it != alphaShape.alpha_shape_facets_end(); ++it) {
      if (alphaShape.classify(*it) == AlphaShape::REGULAR) {
        auto triangle = alphaShape.triangle(*it);

        auto v0 = triangle.vertex(0);
        auto v1 = triangle.vertex(1);
        auto v2 = triangle.vertex(2);

        mesh->add_face(mesh->add_vertex(v0), mesh->add_vertex(v1), mesh->add_vertex(v2));
      }
    }
    return mesh;
  };
}  // namespace to_geom::calculator::AlphaShapeCalculator
