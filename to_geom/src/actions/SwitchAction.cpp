#include "SwitchAction.hpp"

#include <memory>

#include "Logger.hpp"
#include "MeshTaskConversionContext.hpp"

namespace to_geom::action {

  SwitchAction::SwitchAction(SwitchProperties properties) : m_properties(properties) {}

  std::shared_ptr<to_geom::conversion_context::MeshTaskConversionContext> SwitchAction::Execute() {
    vrml_proc::core::logger::LogDebug("Execute SwitchAction.", LOGGING_INFO);
    return m_properties.choice;
  }
}  // namespace to_geom::action
