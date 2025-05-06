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
#include "TraversorResult.hpp"
#include "HandlerToActionBundle.hpp"
#include "ConversionContextable.hpp"

namespace vrml_proc::traversor::handler::ElevationGridHandler {

  template <ConversionContextable ConversionContext>
  TraversorResult<ConversionContext> Handle(vrml_proc::traversor::VrmlNodeTraversorParameters context,
      const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap,
      std::shared_ptr<vrml_proc::traversor::node_descriptor::NodeView> nd) {  //

    using namespace vrml_proc::traversor::handler::HandlerUtils;

    vrml_proc::core::logger::LogDebug(
        vrml_proc::core::utils::FormatString("Handle VRML node <", context.node.header, ">."), LOGGING_INFO);

    // There are geometry primitive nodes 'coord', 'color', 'normal' and 'texCoord'. They are sent as VrmlNodes into the
    // given action where they can be traversed if needed. The main point is they are not traversed inside this handler.
    // But they have to be validated nonetheless.
    {
      auto validationResult = ValidateGeometryPrimitiveNode(nd, context.manager, "color");
      if (validationResult.has_error()) {
        return cpp::fail(validationResult.error());
      }
    }
    {
      auto validationResult = ValidateGeometryPrimitiveNode(nd, context.manager, "normal");
      if (validationResult.has_error()) {
        return cpp::fail(validationResult.error());
      }
    }
    {
      auto validationResult = ValidateGeometryPrimitiveNode(nd, context.manager, "texCoord");
      if (validationResult.has_error()) {
        return cpp::fail(validationResult.error());
      }
    }

    nd->SetShapeDescendant(context.IsDescendantOfShape);
    nd->SetTransformationMatrix(context.transformation);
    auto data = HandlerToActionBundle<ConversionContext>(nd);
    data.config = context.config;

    return vrml_proc::traversor::utils::ConversionContextActionExecutor::TryToExecute<ConversionContext>(
        actionMap, nd->GetId(), data);
  }
}  // namespace vrml_proc::traversor::handler::ElevationGridHandler