#pragma once

#include <memory>

#include "ConversionContextable.hpp"
#include "ConversionContextActionExecutor.hpp"
#include "ConversionContextActionMap.hpp"
#include "FormatString.hpp"
#include "HandlerParameters.hpp"
#include "HandlerToActionBundle.hpp"
#include "Logger.hpp"
#include "TraversorResult.hpp"
#include "VrmlNodeTraversorParameters.hpp"

namespace vrml_proc::traversor::handler::BasicHandler {
  /**
   * @brief Handles the given VRML node by dispatching it to the corresponding action
   *        defined in the provided action map.
   *
   * This `BasicHandler` can be used if node does not requiere any calls of VrmlTraversor on any child, meaning that it
   * does not contain any recursive VRML node children. It sets to NodeView
   * object a current transformation matrix and shape descendant flag; it creates HandlerToActionBundle object and fills
   * it with configuration file and NodeView and tries to execute given action.
   *
   * @tparam ConversionContext type that satisfies the ConversionContextable concept
   * @param params handler parameters
   * @return A TraversorResult containing either the resulting ConversionContext or an error
   *         if the node was unhandled or an error occurred during processing
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