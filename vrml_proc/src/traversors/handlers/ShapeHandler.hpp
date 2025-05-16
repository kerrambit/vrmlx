#pragma once

#include <memory>

#include <result.hpp>

#include "ConversionContextActionExecutor.hpp"
#include "ConversionContextActionMap.hpp"
#include "Error.hpp"
#include "FormatString.hpp"
#include "VrmlNodeTraversorParameters.hpp"
#include "Logger.hpp"
#include "NodeTraversorError.hpp"
#include "VrmlNode.hpp"
#include "NodeDescriptor.hpp"
#include "TraversorResult.hpp"
#include "HandlerToActionBundle.hpp"
#include "ConversionContextable.hpp"
#include "HandlerParameters.hpp"

// Forward declaration.
namespace vrml_proc::traversor {
  template <ConversionContextable ConversionContext>
  class VrmlNodeTraversor;
}  // namespace vrml_proc::traversor

namespace vrml_proc::traversor::handler::ShapeHandler {
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

    using vrml_proc::parser::model::VrmlNode;

    // ---------------------------------------------------

    vrml_proc::core::logger::LogDebug(
        vrml_proc::core::utils::FormatString("Handle VRML node <", params.nodeView->GetName(), ">."), LOGGING_INFO);

    VrmlNodeTraversor<ConversionContext> traversor(params.manager, params.config, params.actionMap, params.headersMap);

    auto traversorParams = VrmlNodeTraversorParameters(
        params.nodeView->template GetField<std::reference_wrapper<const VrmlNode>>("appearance"), true,
        params.transformation);

    auto resolvedAppearance = traversor.Traverse(traversorParams);
    if (resolvedAppearance.has_error()) {
      return cpp::fail(resolvedAppearance.error());
    }

    traversorParams = VrmlNodeTraversorParameters(
        params.nodeView->template GetField<std::reference_wrapper<const VrmlNode>>("geometry"), true,
        params.transformation);

    auto resolvedGeometry = traversor.Traverse(traversorParams);
    if (resolvedGeometry.has_error()) {
      return cpp::fail(resolvedGeometry.error());
    }

    // ---------------------------------------------------

    params.nodeView->SetShapeDescendant(params.IsDescendantOfShape);
    params.nodeView->SetTransformationMatrix(params.transformation);
    auto data = HandlerToActionBundle<ConversionContext>(params.nodeView);
    data.cc1 = resolvedAppearance.value();
    data.cc2 = resolvedGeometry.value();
    data.config = params.config;

    return vrml_proc::traversor::utils::ConversionContextActionExecutor::TryToExecute<ConversionContext>(
        params.actionMap, params.nodeView->GetName(), data);
  }
}  // namespace vrml_proc::traversor::handler::ShapeHandler