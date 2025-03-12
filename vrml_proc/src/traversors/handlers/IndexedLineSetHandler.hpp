#pragma once

#include <any>
#include <memory>

#include <result.hpp>

#include "ConversionContextActionExecutor.hpp"
#include "ConversionContextActionMap.hpp"
#include "Error.hpp"
#include "FormatString.hpp"
#include "VrmlNodeTraversorParameters.hpp"
#include "Int32Array.hpp"
#include "Logger.hpp"
#include "NodeTraversorError.hpp"
#include "VrmlNode.hpp"
#include "VrmlUnits.hpp"
#include "VrmlCanonicalHeaders.hpp"
#include "NodeDescriptor.hpp"
#include "NodeDescriptorMap.hpp"

#include "VrmlProcessingExport.hpp"

namespace vrml_proc::traversor::handler::IndexedLineSetHandler {

  template <typename ConversionContext>
  VRMLPROCESSING_API cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>>
  Handle(vrml_proc::traversor::VrmlNodeTraversorParameters context,
         const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap,
         const vrml_proc::traversor::node_descriptor::NodeDescriptor& nd) {
    using namespace vrml_proc::core::logger;
    using namespace vrml_proc::core::utils;
    using namespace vrml_proc::traversor::node_descriptor;
    using namespace vrml_proc::traversor::error;

    LogInfo(FormatString("Handle VRML node <", context.node.header, ">."), LOGGING_INFO);

    // There are geometry primitive nodes 'coord' and 'color'. They are sent as VrmlNodes into the given action where
    // they can be traversed if needed. The main point is they are not traversed inside this handler. But they have to
    // be validated nonetheless.
    {
      auto node = nd.GetField<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>("color");
      auto descriptorMap = CreateNodeDescriptorMap();
      auto it = descriptorMap.find(ConvertToCanonicalName(node.get().header));
      NodeDescriptor nd;
      if (it != descriptorMap.end()) {
        nd = it->second();
        auto validationResult = nd.Validate(node.get(), context.manager);
        if (validationResult.has_error()) {
          LogError(FormatString("Validation for geometry primitive node <", node.get().header, "> failed!"),
                   LOGGING_INFO);
          return cpp::fail(std::make_shared<NodeTraversorError>(validationResult.error(), node.get()));
        }
      }
    }

    {
      auto node = nd.GetField<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>("coord");
      auto descriptorMap = CreateNodeDescriptorMap();
      auto it = descriptorMap.find(ConvertToCanonicalName(node.get().header));
      NodeDescriptor nd;
      if (it != descriptorMap.end()) {
        nd = it->second();
        auto validationResult = nd.Validate(node.get(), context.manager);
        if (validationResult.has_error()) {
          LogError(FormatString("Validation for geometry primitive node <", node.get().header, "> failed!"),
                   LOGGING_INFO);
          return cpp::fail(std::make_shared<NodeTraversorError>(validationResult.error(), node.get()));
        }
      }
    }

    std::any color = nd.GetField<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>("color");
    std::any coord = nd.GetField<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>("coord");
    std::any colorIndex = nd.GetField<std::reference_wrapper<const vrml_proc::parser::Int32Array>>("colorIndex");
    std::any colorPerVertex = nd.GetField<std::reference_wrapper<const bool>>("colorPerVertex");
    std::any coordIndex = nd.GetField<std::reference_wrapper<const vrml_proc::parser::Int32Array>>("coordIndex");

    return vrml_proc::traversor::utils::ConversionContextActionExecutor::TryToExecute<ConversionContext>(
        actionMap, nd.GetId(),
        {
            std::cref(color),
            std::cref(coord),
            std::cref(colorIndex),
            std::cref(colorPerVertex),
            std::cref(coordIndex),
        },
        {context.IsDescendantOfShape, context.transformation});
  }
}  // namespace vrml_proc::traversor::handler::IndexedLineSetHandler