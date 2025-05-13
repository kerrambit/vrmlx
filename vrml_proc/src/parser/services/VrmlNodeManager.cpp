#include "VrmlNodeManager.hpp"

#include <optional>
#include <functional>
#include <string>

#include "Logger.hpp"
#include "FormatString.hpp"

#include "VrmlNode.hpp"

vrml_proc::parser::service::VrmlNodeManager::VrmlNodeManager() {
  vrml_proc::core::logger::LogDebug("Construct new VrmlNodeManager.", LOGGING_INFO);
}

void vrml_proc::parser::service::VrmlNodeManager::AddDefinitionNode(
    const std::string& id, const vrml_proc::parser::VrmlNode& node) {  //

  vrml_proc::core::logger::LogDebug(
      vrml_proc::core::utils::FormatString("Add DEF node with id <", id, "> into map."), LOGGING_INFO);
  vrml_proc::core::logger::LogDebug(
      vrml_proc::core::utils::FormatString("Address of VRML node: <", &node, ">."), LOGGING_INFO);

  auto result = m_definitionNodes.insert_or_assign(
      id, std::reference_wrapper<const vrml_proc::parser::VrmlNode>(std::cref(node)));
  if (!result.second) {
    vrml_proc::core::logger::LogDebug(
        vrml_proc::core::utils::FormatString(
            "Id <", id, "> was already present in the map. The value was replaced with the new entity."),
        LOGGING_INFO);
  }
}

std::optional<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>
vrml_proc::parser::service::VrmlNodeManager::GetDefinitionNode(const std::string& id) const {  //

  vrml_proc::core::logger::LogDebug(
      vrml_proc::core::utils::FormatString("Retrieve DEF node with id <", id, "> from the map."), LOGGING_INFO);
  auto it = m_definitionNodes.find(id);
  if (it != m_definitionNodes.end()) {
    return it->second;
  }

  vrml_proc::core::logger::LogDebug(
      vrml_proc::core::utils::FormatString("DEF node with id <", id, "> was not found in the map!"), LOGGING_INFO);
  return {};
}
