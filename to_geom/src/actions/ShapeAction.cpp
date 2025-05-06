#include "ShapeAction.hpp"

#include <memory>

#include "Logger.hpp"
#include "MeshTaskConversionContext.hpp"

namespace to_geom {
  namespace action {

    ShapeAction::ShapeAction(Properties properties) : m_properties(properties) {}

    std::shared_ptr<to_geom::conversion_context::MeshTaskConversionContext> ShapeAction::Execute() {
      vrml_proc::core::logger::LogDebug("Execute ShapeAction.", LOGGING_INFO);
      return m_properties.geometry;
    }
  }  // namespace action
}  // namespace to_geom
