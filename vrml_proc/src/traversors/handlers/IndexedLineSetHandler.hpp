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
#include "HandlerUtils.hpp"

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
    using namespace vrml_proc::traversor::handler::HandlerUtils;

    LogInfo(FormatString("Handle VRML node <", context.node.header, ">."), LOGGING_INFO);

    // There are geometry primitive nodes 'coord' and 'color'. They are sent as VrmlNodes into the given action where
    // they can be traversed if needed. The main point is they are not traversed inside this handler. But they have to
    // be validated nonetheless.
    {
      auto validationResult = ValidateGeometryPrimitiveNode(nd, context.manager, "color");
      if (validationResult.has_error()) {
        return cpp::fail(validationResult.error());
      }
    }

    {
      auto validationResult = ValidateGeometryPrimitiveNode(nd, context.manager, "coord");
      if (validationResult.has_error()) {
        return cpp::fail(validationResult.error());
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