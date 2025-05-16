#pragma once

#include <memory>
#include <optional>
#include <string>

#include <result.hpp>

#include "Error.hpp"
#include "FormatString.hpp"
#include "Logger.hpp"
#include "NodeDescriptor.hpp"
#include "NodeDescriptorMap.hpp"
#include "NodeTraversorError.hpp"
#include "VrmlHeaders.hpp"
#include "VrmlNode.hpp"
#include "VrmlNodeManager.hpp"

namespace vrml_proc::traversor::handler::HandlerUtils {

  /**
   * @brief Validates given geometry primitive node.
   *
   * @param nodeView node view
   * @param manager manager
   * @param fieldName field name to verify
   * @param headersMap map with all header (synonyms to canonical) names
   *
   * @returns error if validation fails, otherwise void
   */
  inline cpp::result<void, std::shared_ptr<vrml_proc::core::error::Error>> ValidateGeometryPrimitiveNode(
      std::shared_ptr<vrml_proc::traversor::node_descriptor::NodeView> nodeView,
      const vrml_proc::parser::service::VrmlNodeManager& manager,
      const std::string& fieldName,
      const vrml_proc::traversor::node_descriptor::VrmlHeaders& headersMap) {  //

    using namespace vrml_proc::core::logger;
    using namespace vrml_proc::core::utils;
    using namespace vrml_proc::traversor::node_descriptor;
    using namespace vrml_proc::traversor::error;

    auto node = nodeView->GetField<std::reference_wrapper<const vrml_proc::parser::model::VrmlNode>>(fieldName);
    auto ndResult = CreateNodeDescriptor(headersMap.ConvertToCanonicalHeader(node.get().header));

    if (ndResult.has_value()) {
      auto validationResult = ndResult.value().Validate(node.get(), manager, headersMap, false);
      if (validationResult.has_error()) {
        LogError(FormatString("Validation for geometry primitive node <", node.get().header, "> from field <",
                     fieldName, "> failed!"),
            LOGGING_INFO);
        return cpp::fail(std::make_shared<NodeTraversorError>(validationResult.error(), node.get()));
      }
    }
    return {};
  }

}  // namespace vrml_proc::traversor::handler::HandlerUtils
