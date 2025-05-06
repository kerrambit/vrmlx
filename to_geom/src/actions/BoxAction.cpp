#include "BoxAction.hpp"

#include <functional>
#include <memory>

#include "BoxCalculator.hpp"
#include "GeometryAction.hpp"
#include "Logger.hpp"
#include "MeshTaskConversionContext.hpp"
#include "TransformationMatrix.hpp"
#include "Vec3f.hpp"

namespace to_geom {
  namespace action {

    BoxAction::BoxAction(Properties properties, GeometryAction::Properties geometryProperties)
        : to_geom::action::GeometryAction(geometryProperties), m_properties(properties) {}

    std::shared_ptr<to_geom::conversion_context::MeshTaskConversionContext> BoxAction::Execute() {
      vrml_proc::core::logger::LogDebug("Execute BoxAction.", LOGGING_INFO);

      auto result = std::make_shared<to_geom::conversion_context::MeshTaskConversionContext>();
      if (!m_geometryProperties.containedByShape) {
        vrml_proc::core::logger::LogDebug(
            "Return empty data because Box node is not a child of a Shape node.", LOGGING_INFO);
        return result;
      }

      std::reference_wrapper<const vrml_proc::parser::Vec3f> size = m_properties.size;
      vrml_proc::math::TransformationMatrix matrix = m_geometryProperties.matrix;

      result->Add([=]() {
        to_geom::calculator::BoxCalculator calculator = to_geom::calculator::BoxCalculator();
        return calculator.Generate3DMesh(size, matrix);
      });

      return result;
    }
  }  // namespace action
}  // namespace to_geom
