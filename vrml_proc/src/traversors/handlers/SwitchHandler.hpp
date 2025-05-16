#pragma once

#include <cstdint>
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
#include "VrmlNodeTraversorParameters.hpp"
#include "HandlerParameters.hpp"

// Forward declaration.
namespace vrml_proc::traversor {
  template <ConversionContextable ConversionContext>
  class VrmlNodeTraversor;
}  // namespace vrml_proc::traversor

namespace vrml_proc::traversor::handler::SwitchHandler {
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
    using namespace vrml_proc::core::logger;
    using namespace vrml_proc::core::utils;

    LogDebug(FormatString("Handle VRML node <", params.nodeView->GetName(), ">."), LOGGING_INFO);

    const int32_t& whichChoice =
        (params.nodeView->template GetField<std::reference_wrapper<const int32_t>>("whichChoice")).get();
    std::shared_ptr<ConversionContext> resolvedChild = std::make_shared<ConversionContext>();

    VrmlNodeTraversor<ConversionContext> traversor(params.manager, params.config, params.actionMap, params.headersMap);

    size_t choiceSize =
        params.nodeView->template GetField<std::vector<std::reference_wrapper<const VrmlNode>>>("choice").size();
    if ((whichChoice >= 0) && (choiceSize != 0) && (whichChoice <= (choiceSize - 1))) {  //

      auto traversorParams = VrmlNodeTraversorParameters(
          params.nodeView->template GetField<std::vector<std::reference_wrapper<const VrmlNode>>>("choice").at(
              whichChoice),
          params.IsDescendantOfShape, params.transformation);
      auto recursiveResult = traversor.Traverse(traversorParams);
      if (recursiveResult.has_error()) {
        return cpp::fail(recursiveResult.error());
      }

      resolvedChild = recursiveResult.value();
    } else {
      LogWarning(FormatString("Skipping traversing VRML node in 'choice' field, as 'whichChoice' value is <",
                     whichChoice, "> and is out of range!"),
          LOGGING_INFO);
    }

    params.nodeView->SetShapeDescendant(params.IsDescendantOfShape);
    params.nodeView->SetTransformationMatrix(params.transformation);
    auto data = HandlerToActionBundle<ConversionContext>(params.nodeView);
    data.cc1 = resolvedChild;
    data.config = params.config;

    return vrml_proc::traversor::utils::ConversionContextActionExecutor::TryToExecute<ConversionContext>(
        params.actionMap, params.nodeView->GetName(), data);
  }
}  // namespace vrml_proc::traversor::handler::SwitchHandler