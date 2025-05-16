#pragma once

#include <memory>

#include <result.hpp>

#include "ConversionContextable.hpp"
#include "ConversionContextActionExecutor.hpp"
#include "ConversionContextActionMap.hpp"
#include "Error.hpp"
#include "FormatString.hpp"
#include "HandlerParameters.hpp"
#include "HandlerToActionBundle.hpp"
#include "HandlerUtils.hpp"
#include "Int32Array.hpp"
#include "Logger.hpp"
#include "NodeTraversorError.hpp"
#include "TraversorResult.hpp"
#include "VrmlNode.hpp"
#include "VrmlNodeTraversorParameters.hpp"
#include "VrmlUnits.hpp"

namespace vrml_proc::traversor::handler::PointSetHandler {
  /**
   * @brief Handles the given VRML node by dispatching it to the corresponding action
   *        defined in the provided action map.
   *
   * This function retrieves the appropriate action for the current node's header hash
   * from `actionMap` and invokes it using the provided parameters. It handles node-specific
   * logic and delegates actual processing to the matching action.
   *
   * @tparam ConversionContext type that satisfies the ConversionContextable concept
   * @param params handler parameters
   * @return A TraversorResult containing either the resulting ConversionContext or an error
   *         if the node was unhandled or an error occurred during processing
   */
  template <ConversionContextable ConversionContext>
  TraversorResult<ConversionContext> Handle(HandlerParameters<ConversionContext> params) {  //

    using namespace vrml_proc::traversor::handler::HandlerUtils;

    // ---------------------------------------------------

    vrml_proc::core::logger::LogDebug(
        vrml_proc::core::utils::FormatString("Handle VRML node <", params.node.header, ">."), LOGGING_INFO);

    // There are geometry primitive nodes 'coord' and 'color'. They are sent as VrmlNodes into the
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

    // ---------------------------------------------------

    params.nodeView->SetShapeDescendant(params.IsDescendantOfShape);
    params.nodeView->SetTransformationMatrix(params.transformation);
    auto data = HandlerToActionBundle<ConversionContext>(params.nodeView);
    data.config = params.config;

    return vrml_proc::traversor::utils::ConversionContextActionExecutor::TryToExecute<ConversionContext>(
        params.actionMap, params.nodeView->GetName(), data);
  }
}  // namespace vrml_proc::traversor::handler::PointSetHandler