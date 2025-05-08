#pragma once

#include <any>
#include <memory>

#include <result.hpp>

#include "ConversionContextable.hpp"
#include "ConversionContextActionExecutor.hpp"
#include "ConversionContextActionMap.hpp"
#include "Error.hpp"
#include "FormatString.hpp"
#include "HandlerToActionBundle.hpp"
#include "HandlerUtils.hpp"
#include "Int32Array.hpp"
#include "Logger.hpp"
#include "NodeTraversorError.hpp"
#include "TraversorResult.hpp"
#include "VrmlNode.hpp"
#include "VrmlNodeTraversorParameters.hpp"
#include "VrmlUnits.hpp"

namespace vrml_proc::traversor::handler::IndexedFaceSetHandler {
  /**
   * @brief Handles given node represented by `nd` and calls appropriate action for it stored in `actionMap`.
   *
   * @tparam ConversionContext type of conversion params
   * @param params parameters received from traversor
   * @param actionMap action map
   * @param nd current node view of the node
   *
   * @returns ConversionContext object, or error if there is some error (in handler or in action)
   */
  template <ConversionContextable ConversionContext>
  TraversorResult<ConversionContext> Handle(vrml_proc::traversor::VrmlNodeTraversorParameters params,
      const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap,
      std::shared_ptr<vrml_proc::traversor::node_descriptor::NodeView> nd) {  //

    using namespace vrml_proc::traversor::handler::HandlerUtils;

    vrml_proc::core::logger::LogDebug(
        vrml_proc::core::utils::FormatString("Handle VRML node <", params.node.header, ">."), LOGGING_INFO);

    // There are geometry primitive nodes 'coord', 'color', 'normal' and 'texCoord'. They are sent as VrmlNodes into the
    // given action where they can be traversed if needed. The main point is they are not traversed inside this handler.
    // But they have to be validated nonetheless.
    {
      auto validationResult = ValidateGeometryPrimitiveNode(nd, params.manager, "color");
      if (validationResult.has_error()) {
        return cpp::fail(validationResult.error());
      }
    }
    {
      auto validationResult = ValidateGeometryPrimitiveNode(nd, params.manager, "coord");
      if (validationResult.has_error()) {
        return cpp::fail(validationResult.error());
      }
    }
    {
      auto validationResult = ValidateGeometryPrimitiveNode(nd, params.manager, "normal");
      if (validationResult.has_error()) {
        return cpp::fail(validationResult.error());
      }
    }
    {
      auto validationResult = ValidateGeometryPrimitiveNode(nd, params.manager, "texCoord");
      if (validationResult.has_error()) {
        return cpp::fail(validationResult.error());
      }
    }

    nd->SetShapeDescendant(params.IsDescendantOfShape);
    nd->SetTransformationMatrix(params.transformation);
    auto data = HandlerToActionBundle<ConversionContext>(nd);
    data.config = params.config;

    return vrml_proc::traversor::utils::ConversionContextActionExecutor::TryToExecute<ConversionContext>(
        actionMap, nd->GetName(), data);
  }
}  // namespace vrml_proc::traversor::handler::IndexedFaceSetHandler