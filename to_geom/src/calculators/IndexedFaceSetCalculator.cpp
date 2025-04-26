#include "IndexedFaceSetCalculator.hpp"

#include <cstdint>
#include <memory>
#include <vector>

#include <CGAL/Kernel/interface_macros.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh/Surface_mesh.h>

#include <result.hpp>

#include "CalculatorError.hpp"
#include "CGALBaseTypesForVrml.hpp"
#include "Vec3f.hpp"
#include "Error.hpp"
#include "FormatString.hpp"
#include "Int32Array.hpp"
#include "Logger.hpp"
#include "ManualTimer.hpp"
#include "Mesh.hpp"
#include "ModelValidationError.hpp"
#include "Range.hpp"
#include "UnsupportedOperationError.hpp"
#include "CalculatorResult.hpp"
#include "IndexedTriangularFaceSetCalculator.hpp"

static to_geom::calculator::CalculatorResult ReturnVertexIndexOutOfRangeError(
    const vrml_proc::core::utils::Range<int32_t>& range, int32_t actualValue) {  //

  using namespace to_geom::calculator::error;
  using vrml_proc::parser::model::validator::error::NumberOutOfRangeError;

  return cpp::fail(std::make_shared<IndexedFaceSetCalculatorError>()
                   << (std::make_shared<PropertiesError>()
                          << (std::make_shared<VertexIndexOutOfRangeError>()
                                 << std::make_shared<NumberOutOfRangeError<int32_t>>(range, actualValue))));
}

namespace to_geom::calculator {
  to_geom::calculator::CalculatorResult IndexedFaceSetCalculator::Generate3DMesh(
      std::reference_wrapper<const vrml_proc::parser::Int32Array> coordinateIndices,
      std::reference_wrapper<const vrml_proc::parser::Vec3fArray> coordinates,
      const vrml_proc::math::TransformationMatrix& matrix,
      bool checkRange,
      bool onlyTriangularFaces) {  //

    // When it is guaranteed that coordinate indices form only triangular faces, we can call specializedand optimazed
    // calculator.
    if (onlyTriangularFaces) {
      to_geom::calculator::IndexedTriangularFaceSetCalculator calculator;
      return calculator.Generate3DMesh(coordinateIndices, coordinates, matrix, checkRange);
    }

    using to_geom::calculator::error::IndexedFaceSetCalculatorError;
    using to_geom::calculator::error::InvalidNumberOfCoordinatesForFaceError;
    using to_geom::calculator::error::PropertiesError;
    using vrml_proc::core::error::UnsupportedOperationError;
    using vrml_proc::core::utils::FormatString;
    using vrml_proc::parser::model::validator::error::EmptyArrayError;
    using namespace vrml_proc::core::logger;
    using vrml_proc::math::cgal::CGALPoint;

    LogInfo("Generate 3D mesh using IndexedFaceSetCalculator.", LOGGING_INFO);

    auto mesh = std::make_shared<to_geom::core::Mesh>();
    auto error = std::make_shared<IndexedFaceSetCalculatorError>();

    const std::vector<int32_t>& indicides = coordinateIndices.get().integers;
    const std::vector<vrml_proc::parser::Vec3f>& points = coordinates.get().vectors;

    if (indicides.empty()) {
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

    size_t start = 0;
    size_t end = 0;
    vrml_proc::core::utils::Range<int32_t> range(0, points.size() - 1);
    for (size_t i = 0; i < indicides.size(); ++i) {
      if (indicides[i] == -1) {
        size_t coordinatesPerFace = end - start;

        if (coordinatesPerFace <= 2) {
          return cpp::fail(error << (std::make_shared<PropertiesError>()
                                     << std::make_shared<InvalidNumberOfCoordinatesForFaceError>(coordinatesPerFace)));
        }

        if (coordinatesPerFace == 3) {
          if (checkRange) {
            if (!range.CheckValueInRangeInclusive(indicides[start])) {
              return ReturnVertexIndexOutOfRangeError(range, indicides[start]);
            }
            if (!range.CheckValueInRangeInclusive(indicides[start + 1])) {
              return ReturnVertexIndexOutOfRangeError(range, indicides[start + 1]);
            }
            if (!range.CheckValueInRangeInclusive(indicides[start + 2])) {
              return ReturnVertexIndexOutOfRangeError(range, indicides[start + 2]);
            }
          }

          mesh->add_face(mesh->add_vertex(matrix.transform(CGALPoint(
                             points[indicides[start]].x, points[indicides[start]].y, points[indicides[start]].z))),
              mesh->add_vertex(matrix.transform(CGALPoint(
                  points[indicides[start + 1]].x, points[indicides[start + 1]].y, points[indicides[start + 1]].z))),
              mesh->add_vertex(matrix.transform(CGALPoint(
                  points[indicides[start + 2]].x, points[indicides[start + 2]].y, points[indicides[start + 2]].z))));
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
    LogInfo(FormatString("Mesh was generated successfully. The generation took ", time, " seconds."), LOGGING_INFO);

    return mesh;
  }
}  // namespace to_geom::calculator
