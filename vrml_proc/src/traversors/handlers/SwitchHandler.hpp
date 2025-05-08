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

namespace vrml_proc::traversor::VrmlNodeTraversor {
  template <ConversionContextable ConversionContext>
  cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> Traverse(
      vrml_proc::traversor::VrmlNodeTraversorParameters context,
      const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap);
}

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
  TraversorResult<ConversionContext> Handle(vrml_proc::traversor::VrmlNodeTraversorParameters params,
      const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap,
      std::shared_ptr<vrml_proc::traversor::node_descriptor::NodeView> nd) {  //

    using namespace vrml_proc::traversor::VrmlNodeTraversor;
    using vrml_proc::parser::VrmlNode;

    vrml_proc::core::logger::LogDebug(
        vrml_proc::core::utils::FormatString("Handle VRML node <", params.node.header, ">."), LOGGING_INFO);

    const int32_t& whichChoice = (nd->GetField<std::reference_wrapper<const int32_t>>("whichChoice")).get();
    std::shared_ptr<ConversionContext> resolvedChild = std::make_shared<ConversionContext>();

    if (whichChoice >= 0 && nd->GetField<std::vector<std::reference_wrapper<const VrmlNode>>>("choice").size() != 0 &&
        whichChoice <= nd->GetField<std::vector<std::reference_wrapper<const VrmlNode>>>("choice").size() - 1) {
      auto recursiveResult = Traverse<ConversionContext>(
          {nd->GetField<std::vector<std::reference_wrapper<const VrmlNode>>>("choice").at(whichChoice), params.manager,
              params.IsDescendantOfShape, params.transformation, params.config},
          actionMap);
      if (recursiveResult.has_error()) {
        return cpp::fail(recursiveResult.error());
      }

      resolvedChild = recursiveResult.value();
    } else {
      vrml_proc::core::logger::LogWarning(
          vrml_proc::core::utils::FormatString(
              "Skipping traversing VRML node in 'choice' field, as 'whichChoice' value is <", whichChoice,
              "> and is out of range!"),
          LOGGING_INFO);
    }

    nd->SetShapeDescendant(params.IsDescendantOfShape);
    nd->SetTransformationMatrix(params.transformation);
    auto data = HandlerToActionBundle<ConversionContext>(nd);
    data.cc1 = resolvedChild;
    data.config = params.config;

    return vrml_proc::traversor::utils::ConversionContextActionExecutor::TryToExecute<ConversionContext>(
        actionMap, nd->GetName(), data);
  }
}  // namespace vrml_proc::traversor::handler::SwitchHandler