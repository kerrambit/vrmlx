#include "GroupAction.hpp"

#include <memory>

#include "Logger.hpp"
#include "MeshTaskConversionContext.hpp"

namespace to_geom {
  namespace action {

    GroupAction::GroupAction(Properties properties) : m_properties(properties) {}

    std::shared_ptr<to_geom::conversion_context::MeshTaskConversionContext> GroupAction::Execute() {
      using to_geom::conversion_context::MeshTaskConversionContext;

      vrml_proc::core::logger::LogDebug("Execute GroupAction.", LOGGING_INFO);

      auto result = std::make_shared<MeshTaskConversionContext>();
      for (const auto& child : m_properties.children) {
        if (child != nullptr) {
          auto meshContextPtr = std::dynamic_pointer_cast<MeshTaskConversionContext>(child);
          if (meshContextPtr != nullptr) {
            result->Merge(meshContextPtr.get());
            meshContextPtr.reset();
          }
        }
      }

      return result;
    }
  }  // namespace action
}  // namespace to_geom
