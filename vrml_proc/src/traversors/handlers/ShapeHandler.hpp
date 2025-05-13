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

// Forward declaration.
namespace vrml_proc::traversor::VrmlNodeTraversor {
  template <ConversionContextable ConversionContext>
  cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> Traverse(
      vrml_proc::traversor::VrmlNodeTraversorParameters context,
      const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap);
}

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
  TraversorResult<ConversionContext> Handle(vrml_proc::traversor::VrmlNodeTraversorParameters params,
      const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap,
      std::shared_ptr<vrml_proc::traversor::node_descriptor::NodeView> nd) {  //

    using namespace vrml_proc::traversor::VrmlNodeTraversor;
    using vrml_proc::parser::model::VrmlNode;

    vrml_proc::core::logger::LogDebug(
        vrml_proc::core::utils::FormatString("Handle VRML node <", params.node.header, ">."), LOGGING_INFO);

    auto resolvedAppearance =
        Traverse<ConversionContext>({nd->GetField<std::reference_wrapper<const VrmlNode>>("appearance").get(),
                                        params.manager, true, params.transformation, params.config},
            actionMap);
    if (resolvedAppearance.has_error()) {
      return cpp::fail(resolvedAppearance.error());
    }

    auto resolvedGeometry =
        Traverse<ConversionContext>({nd->GetField<std::reference_wrapper<const VrmlNode>>("geometry").get(),
                                        params.manager, true, params.transformation, params.config},
            actionMap);
    if (resolvedGeometry.has_error()) {
      return cpp::fail(resolvedGeometry.error());
    }

    nd->SetShapeDescendant(params.IsDescendantOfShape);
    nd->SetTransformationMatrix(params.transformation);
    auto data = HandlerToActionBundle<ConversionContext>(nd);
    data.cc1 = resolvedAppearance.value();
    data.cc2 = resolvedGeometry.value();
    data.config = params.config;

    return vrml_proc::traversor::utils::ConversionContextActionExecutor::TryToExecute<ConversionContext>(
        actionMap, nd->GetName(), data);
  }
}  // namespace vrml_proc::traversor::handler::ShapeHandler