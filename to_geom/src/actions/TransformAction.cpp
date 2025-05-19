#include "TransformAction.hpp"

#include <memory>

#include "Logger.hpp"
#include "MeshTaskConversionContext.hpp"

namespace to_geom::action {

  TransformAction::TransformAction(Properties properties) : m_properties(properties) {}

  std::shared_ptr<to_geom::conversion_context::MeshTaskConversionContext> TransformAction::Execute() {
    vrml_proc::core::logger::LogDebug("Execute TransformAction.", LOGGING_INFO);

    auto result = std::make_shared<to_geom::conversion_context::MeshTaskConversionContext>();
    for (auto child : m_properties.children) {
      result->Merge(child.get());
      child.reset();
    }

    return result;
  }
}  // namespace to_geom::action
