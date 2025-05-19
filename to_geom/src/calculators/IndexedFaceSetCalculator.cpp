#include "IndexedFaceSetCalculator.hpp"

#include <cstdint>
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

#include <CGAL/Kernel/interface_macros.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh/Surface_mesh.h>

#include <result.hpp>

#include "CalculatorError.hpp"
#include "CalculatorResult.hpp"
#include "CalculatorUtils.hpp"
#include "CGALBaseTypesForVrml.hpp"
#include "Error.hpp"
#include "FormatString.hpp"
#include "Int32Array.hpp"
#include "Logger.hpp"
#include "ManualTimer.hpp"
#include "Mesh.hpp"
#include "Range.hpp"
#include "UnsupportedOperationError.hpp"
#include "Vec3f.hpp"
#include "Vec3fArray.hpp"

namespace to_geom::calculator {

  to_geom::calculator::CalculatorResult IndexedFaceSetCalculator::Generate3DMesh(
      std::reference_wrapper<const vrml_proc::parser::model::Int32Array> coordinateIndices,
      std::reference_wrapper<const vrml_proc::parser::model::Vec3fArray> coordinates,
      const vrml_proc::math::TransformationMatrix& matrix,
      bool checkRange) {  //

    using to_geom::calculator::error::IndexedFaceSetCalculatorError;
    using to_geom::calculator::error::InvalidNumberOfCoordinatesForFaceError;
    using to_geom::calculator::error::PropertiesError;
    using vrml_proc::core::error::UnsupportedOperationError;
    using vrml_proc::core::utils::FormatString;
    using vrml_proc::parser::model::validator::error::EmptyArrayError;
    using namespace vrml_proc::core::logger;
    using vrml_proc::math::cgal::CGALPoint;

    LogDebug("Generate 3D mesh using IndexedFaceSetCalculator.", LOGGING_INFO);

    auto mesh = std::make_shared<to_geom::core::Mesh>();
    auto error = std::make_shared<IndexedFaceSetCalculatorError>();

    const std::vector<int32_t>& indices = coordinateIndices.get().integers;
    const std::vector<vrml_proc::parser::model::Vec3f>& points = coordinates.get().vectors;

    if (indices.empty()) {
      LogWarning("No coordinate indices were found! There is nothing to calculate and empty mesh will be returned.",
          LOGGING_INFO);
      return mesh;
    }
    if (points.empty()) {
      return cpp::fail(
          error << (std::make_shared<PropertiesError>() << std::make_shared<EmptyArrayError>("coordinates")));
    }

    auto timer = vrml_proc::core::utils::ManualTimer();
    timer.Start();

    // Map indices to CGAL vertices.
    std::unordered_map<int32_t, typename to_geom::core::Mesh::Vertex_index> indexToVertex;

    vrml_proc::core::utils::Range<int32_t> range(0, points.size() - 1);

    size_t start = 0;
    size_t end = 0;
    for (size_t i = 0; i < indices.size(); ++i) {
      if (indices[i] == -1) {  //

        size_t coordinatesPerFace = end - start;

        if (coordinatesPerFace <= 2) {
          return cpp::fail(error << (std::make_shared<PropertiesError>()
                                     << std::make_shared<InvalidNumberOfCoordinatesForFaceError>(coordinatesPerFace)));
        }

        if (coordinatesPerFace == 3) {  //

          typename to_geom::core::Mesh::Vertex_index v1;
          {
            auto it = indexToVertex.find(indices[start]);
            if (it == indexToVertex.end()) {
              if (checkRange && !range.CheckValueInRangeInclusive(indices[start])) {
                return CalculatorUtils::ReturnVertexIndexOutOfRangeError<IndexedFaceSetCalculatorError>(
                    range, indices[start]);
              }
              v1 = mesh->add_vertex(matrix.transform(
                  CGALPoint(points[indices[start]].x, points[indices[start]].y, points[indices[start]].z)));
              indexToVertex[indices[start]] = v1;
            } else {
              v1 = it->second;
            }
          }

          typename to_geom::core::Mesh::Vertex_index v2;
          {
            auto it = indexToVertex.find(indices[start + 1]);
            if (it == indexToVertex.end()) {
              if (checkRange && !range.CheckValueInRangeInclusive(indices[start + 1])) {
                return CalculatorUtils::ReturnVertexIndexOutOfRangeError<IndexedFaceSetCalculatorError>(
                    range, indices[start + 1]);
              }
              v2 = mesh->add_vertex(matrix.transform(
                  CGALPoint(points[indices[start + 1]].x, points[indices[start + 1]].y, points[indices[start + 1]].z)));
              indexToVertex[indices[start + 1]] = v2;
            } else {
              v2 = it->second;
            }
          }

          typename to_geom::core::Mesh::Vertex_index v3;
          {
            auto it = indexToVertex.find(indices[start + 2]);
            if (it == indexToVertex.end()) {
              if (checkRange && !range.CheckValueInRangeInclusive(indices[start + 2])) {
                return CalculatorUtils::ReturnVertexIndexOutOfRangeError<IndexedFaceSetCalculatorError>(
                    range, indices[start + 2]);
              }
              v3 = mesh->add_vertex(matrix.transform(
                  CGALPoint(points[indices[start + 2]].x, points[indices[start + 2]].y, points[indices[start + 2]].z)));
              indexToVertex[indices[start + 2]] = v3;
            } else {
              v3 = it->second;
            }
          }

#ifdef DEBUG
          auto face = mesh->add_face(v1, v2, v3);
          if (face == CGAL::Surface_mesh<CGALPoint>::null_face()) {
            LogWarning(FormatString("Failed to add face: ", v1, " (", mesh->point(v1), "), ", v2, " (", mesh->point(v2),
                           "), ", v3, " (", mesh->point(v3), ")"),
                LOGGING_INFO);
          }
#else
          mesh->add_face(v1, v2, v3);
#endif
        } else {
          return cpp::fail(
              std::make_shared<IndexedFaceSetCalculatorError>() << std::make_shared<UnsupportedOperationError>(
                  "Face is constructed from more than 3 coordinates. IndexedFaceSetCalculator does not support fan "
                  "triangulation or any other algorithms for such a task yet. Thus, mesh cannot be generated fully."));
        }

        start = end + 1;
      }

      end++;
    }

    double time = timer.End();
    LogDebug(FormatString("Mesh was generated successfully. The generation took ", time, " seconds."), LOGGING_INFO);

#ifdef DEBUG
    LogDebug(FormatString(
                 "Mesh contains ", mesh->number_of_vertices(), " vertices and ", mesh->number_of_faces(), " faces."),
        LOGGING_INFO);

    LogDebug("Vertex coordinates:", LOGGING_INFO);
    for (auto v : mesh->vertices()) {
      auto point = mesh->point(v);
      LogDebug(FormatString("Vertex ", v, ": (", point.x(), ", ", point.y(), ", ", point.z(), ")"), LOGGING_INFO);
    }

    LogDebug("Face data:", LOGGING_INFO);
    for (auto f : mesh->faces()) {
      std::ostringstream oss;
      oss << "Face " << f << " contains vertices: ";
      for (auto v : vertices_around_face(mesh->halfedge(f), *mesh)) {
        oss << v << " ";
      }
      LogDebug(oss.str(), LOGGING_INFO);
    }
#endif

    return mesh;
  }
}  // namespace to_geom::calculator
