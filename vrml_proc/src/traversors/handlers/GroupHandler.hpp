#pragma once

#include <memory>

#include <result.hpp>

#include "ConversionContextable.hpp"
#include "ConversionContextActionExecutor.hpp"
#include "ConversionContextActionMap.hpp"
#include "Error.hpp"
#include "FormatString.hpp"
#include "HandlerToActionBundle.hpp"
#include "Logger.hpp"
#include "NodeDescriptor.hpp"
#include "NodeTraversorError.hpp"
#include "TraversorResult.hpp"
#include "Vec3f.hpp"
#include "VrmlNodeTraversorParameters.hpp"
#include "HandlerParameters.hpp"

// Forward declaration.
namespace vrml_proc::traversor {
  template <ConversionContextable ConversionContext>
  class VrmlNodeTraversor;
}  // namespace vrml_proc::traversor

namespace vrml_proc::traversor::handler::GroupHandler {
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

    using namespace vrml_proc::core::logger;
    using namespace vrml_proc::core::utils;
    using vrml_proc::parser::model::VrmlNode;

    LogDebug(FormatString("Handle VRML node <", params.nodeView->GetName(), ">."), LOGGING_INFO);
    auto traversor = vrml_proc::traversor::VrmlNodeTraversor<ConversionContext>(
        params.manager, params.config, params.actionMap, params.headersMap);

    std::vector<std::shared_ptr<ConversionContext>> resolvedChildren;
    for (const auto& child :
        params.nodeView->template GetField<std::vector<std::reference_wrapper<const VrmlNode>>>("children")) {
      auto traversorParams = VrmlNodeTraversorParameters(child, params.IsDescendantOfShape, params.transformation);
      auto recursiveResult = traversor.Traverse(traversorParams);

      if (recursiveResult.has_error()) {
        return cpp::fail(recursiveResult.error());
      }

      resolvedChildren.push_back(recursiveResult.value());
    }

    params.nodeView->SetShapeDescendant(params.IsDescendantOfShape);
    params.nodeView->SetTransformationMatrix(params.transformation);

    auto data = HandlerToActionBundle<ConversionContext>(params.nodeView);
    data.ccGroup = resolvedChildren;
    data.config = params.config;

    return vrml_proc::traversor::utils::ConversionContextActionExecutor::TryToExecute<ConversionContext>(
        params.actionMap, params.nodeView->GetName(), data);
  }
}  // namespace vrml_proc::traversor::handler::GroupHandler