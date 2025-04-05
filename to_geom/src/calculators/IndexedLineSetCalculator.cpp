#include "IndexedLineSetCalculator.hpp"

#include <cstdint>
#include <memory>
#include <vector>

#include <CGAL/Kernel/interface_macros.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh/Surface_mesh.h>

#include <result.hpp>

#include "CalculatorError.hpp"
#include "CGALBaseTypesForVrml.hpp"
#include "Error.hpp"
#include "FormatString.hpp"
#include "Int32Array.hpp"
#include "Logger.hpp"
#include "ManualTimer.hpp"
#include "Mesh.hpp"
#include "ModelValidationError.hpp"
#include "Range.hpp"
#include "CalculatorResult.hpp"
#include "UnsupportedOperationError.hpp"

namespace to_geom::calculator {
  to_geom::calculator::CalculatorResult IndexedLineSetCalculator::Generate3DMesh(
      std::reference_wrapper<const vrml_proc::parser::Int32Array> coordinateIndices,
      std::reference_wrapper<const vrml_proc::parser::Vec3fArray> coordinates,
      const vrml_proc::math::TransformationMatrix& matrix) {  //

    using namespace vrml_proc::core::logger;
    using vrml_proc::core::error::UnsupportedOperationError;
    using vrml_proc::parser::Vec3f;
    using vrml_proc::parser::model::validator::error::EmptyArrayError;

    LogInfo("Generate 3D mesh using IndexedLineSetCalculator.", LOGGING_INFO);

    auto mesh = std::make_shared<core::Mesh>();
    auto error = std::make_shared<error::IndexedLineSetCalculatorError>();

    const std::vector<int32_t>& indicides = coordinateIndices.get().integers;
    const std::vector<Vec3f>& points = coordinates.get().vectors;

    if (indicides.empty()) {
      LogWarning("No coordinate indices were found! There is nothing to calculate and empty mesh will be returned.",
          LOGGING_INFO);
      return mesh;
    }
    if (points.empty()) {
      return cpp::fail(error << (std::make_shared<error::PropertiesError>()
                                 << std::make_shared<EmptyArrayError>("coordinateIndices")));
    }

    return cpp::fail(
        std::make_shared<error::IndexedLineSetCalculatorError>() << std::make_shared<UnsupportedOperationError>(
            "IndexedLineSetCalculator unfortunately does not support generating 3D mesh from polylines."));
  }
}  // namespace to_geom::calculator
