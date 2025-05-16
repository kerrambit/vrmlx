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
#include "HandlerParameters.hpp"

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
  TraversorResult<ConversionContext> Handle(HandlerParameters<ConversionContext> params) {  //

    using namespace vrml_proc::traversor::handler::HandlerUtils;

    vrml_proc::core::logger::LogDebug(
        vrml_proc::core::utils::FormatString("Handle VRML node <", params.nodeView->GetName(), ">."), LOGGING_INFO);

    // There are geometry primitive nodes 'coord', 'color', 'normal' and 'texCoord'. They are sent as VrmlNodes into the
    // given action where they can be traversed if needed. The main point is they are not traversed inside this handler.
    // But they have to be validated nonetheless.
    {
      auto validationResult =
          ValidateGeometryPrimitiveNode(params.nodeView, params.manager, "color", params.headersMap);
      if (validationResult.has_error()) {
        return cpp::fail(validationResult.error());
      }
    }
    {
      auto validationResult =
          ValidateGeometryPrimitiveNode(params.nodeView, params.manager, "coord", params.headersMap);
      if (validationResult.has_error()) {
        return cpp::fail(validationResult.error());
      }
    }
    {
      auto validationResult =
          ValidateGeometryPrimitiveNode(params.nodeView, params.manager, "normal", params.headersMap);
      if (validationResult.has_error()) {
        return cpp::fail(validationResult.error());
      }
    }
    {
      auto validationResult =
          ValidateGeometryPrimitiveNode(params.nodeView, params.manager, "texCoord", params.headersMap);
      if (validationResult.has_error()) {
        return cpp::fail(validationResult.error());
      }
    }

    params.nodeView->SetShapeDescendant(params.IsDescendantOfShape);
    params.nodeView->SetTransformationMatrix(params.transformation);
    auto data = HandlerToActionBundle<ConversionContext>(params.nodeView);
    data.config = params.config;

    return vrml_proc::traversor::utils::ConversionContextActionExecutor::TryToExecute<ConversionContext>(
        params.actionMap, params.nodeView->GetName(), data);
  }
}  // namespace vrml_proc::traversor::handler::IndexedFaceSetHandler