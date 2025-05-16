#pragma once

#include <memory>

#include "ConversionContextable.hpp"
#include "ConversionContextActionExecutor.hpp"
#include "ConversionContextActionMap.hpp"
#include "FormatString.hpp"
#include "HandlerToActionBundle.hpp"
#include "Logger.hpp"
#include "TraversorResult.hpp"
#include "VrmlNodeTraversorParameters.hpp"
#include "HandlerParameters.hpp"

namespace vrml_proc::traversor::handler::BasicHandler {
  /**
   * @brief Handles given node represented by `nd` and calls appropriate action for it stored in `actionMap`.
   *
   * This `BasicHandler` can be used if node does not requiere any calls of VrmlTraversor on any child, meaning that it
   * does not contain any recursive VRML node children - it contains only basic data types. It then sets to NodeView
   * object a current transformation matrix and shape descendant flag; it creates HandlerToActionBundle object and fills
   * it with configuration file and NodeView and tries to execute given action.
   *
   * @param params parameters received from traversor
   * @param actionMap action map
   * @param nd current node view of the node
   *
   * @returns ConversionContext object, or error if there is some error (in handler or in action)
   */
  template <ConversionContextable ConversionContext>
  TraversorResult<ConversionContext> Handle(HandlerParameters<ConversionContext> params) {  //

    vrml_proc::core::logger::LogDebug(
        vrml_proc::core::utils::FormatString("Handle VRML node <", params.nodeView->GetName(), ">."), LOGGING_INFO);

    params.nodeView->SetShapeDescendant(params.IsDescendantOfShape);
    params.nodeView->SetTransformationMatrix(params.transformation);
    auto data = HandlerToActionBundle<ConversionContext>(params.nodeView);
    data.config = params.config;

    return vrml_proc::traversor::utils::ConversionContextActionExecutor::TryToExecute<ConversionContext>(
        params.actionMap, params.nodeView->GetName(), data);
  }
}  // namespace vrml_proc::traversor::handler::BasicHandler