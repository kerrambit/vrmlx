#pragma once

#include <memory>
#include <string>
#include <optional>

#include <result.hpp>

#include "Error.hpp"
#include "FormatString.hpp"
#include "Logger.hpp"
#include "NodeDescriptor.hpp"
#include "NodeDescriptorMap.hpp"
#include "NodeTraversorError.hpp"
#include "VrmlNode.hpp"
#include "VrmlNodeManager.hpp"

namespace vrml_proc::traversor::handler::HandlerUtils {

  inline cpp::result<void, std::shared_ptr<vrml_proc::core::error::Error>> ValidateGeometryPrimitiveNode(
      std::shared_ptr<vrml_proc::traversor::node_descriptor::NodeView> nd,
      const vrml_proc::parser::VrmlNodeManager& manager,
      const std::string& fieldName) {  //

    using namespace vrml_proc::core::logger;
    using namespace vrml_proc::core::utils;
    using namespace vrml_proc::traversor::node_descriptor;
    using namespace vrml_proc::traversor::error;

    auto node = nd->GetField<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>(fieldName);
    auto ndResult = CreateNodeDescriptor(node.get().header);

    if (ndResult.has_value()) {
      auto validationResult = ndResult.value().Validate(node.get(), manager);
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
