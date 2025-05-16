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

    using vrml_proc::parser::model::VrmlNode;

    vrml_proc::core::logger::LogDebug(
        vrml_proc::core::utils::FormatString("Handle VRML node <", params.nodeView->GetName(), ">."), LOGGING_INFO);

    VrmlNodeTraversor<ConversionContext> traversor(params.manager, params.config, params.actionMap, params.headersMap);

    auto traversorParams = VrmlNodeTraversorParameters(
        params.nodeView->template GetField<std::reference_wrapper<const VrmlNode>>("appearance").get(), true,
        params.transformation);
    auto resolvedAppearance = traversor.Traverse(traversorParams);
    if (resolvedAppearance.has_error()) {
      return cpp::fail(resolvedAppearance.error());
    }

    traversorParams = VrmlNodeTraversorParameters(
        params.nodeView->template GetField<std::reference_wrapper<const VrmlNode>>("geometry").get(), true,
        params.transformation);
    auto resolvedGeometry = traversor.Traverse(traversorParams);
    if (resolvedGeometry.has_error()) {
      return cpp::fail(resolvedGeometry.error());
    }

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