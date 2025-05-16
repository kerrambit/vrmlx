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
    using namespace vrml_proc::core::logger;
    using namespace vrml_proc::core::utils;

    // ---------------------------------------------------

    LogDebug(FormatString("Handle VRML node <", params.nodeView->GetName(), ">."), LOGGING_INFO);

    const int32_t& whichChoice =
        (params.nodeView->template GetField<std::reference_wrapper<const int32_t>>("whichChoice")).get();
    std::shared_ptr<ConversionContext> resolvedChild = std::make_shared<ConversionContext>();

    VrmlNodeTraversor<ConversionContext> traversor(params.manager, params.config, params.actionMap, params.headersMap);

    size_t choiceSize =
        params.nodeView->template GetField<std::vector<std::reference_wrapper<const VrmlNode>>>("choice").size();
    if ((whichChoice >= 0) && (choiceSize != 0) && (whichChoice <= (choiceSize - 1))) {  //

      auto choices = params.nodeView->template GetField<std::vector<std::reference_wrapper<const VrmlNode>>>("choice");
      auto traversorParams =
          VrmlNodeTraversorParameters(choices[whichChoice], params.IsDescendantOfShape, params.transformation);

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

    // ---------------------------------------------------

    params.nodeView->SetShapeDescendant(params.IsDescendantOfShape);
    params.nodeView->SetTransformationMatrix(params.transformation);
    auto data = HandlerToActionBundle<ConversionContext>(params.nodeView);
    data.cc1 = resolvedChild;
    data.config = params.config;

    return vrml_proc::traversor::utils::ConversionContextActionExecutor::TryToExecute<ConversionContext>(
        params.actionMap, params.nodeView->GetName(), data);
  }
}  // namespace vrml_proc::traversor::handler::SwitchHandler