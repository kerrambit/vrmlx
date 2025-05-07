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

  template <ConversionContextable ConversionContext>
  TraversorResult<ConversionContext> Handle(vrml_proc::traversor::VrmlNodeTraversorParameters context,
      const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap,
      std::shared_ptr<vrml_proc::traversor::node_descriptor::NodeView> nd) {  //

    using namespace vrml_proc::traversor::VrmlNodeTraversor;
    using vrml_proc::parser::VrmlNode;

    vrml_proc::core::logger::LogDebug(
        vrml_proc::core::utils::FormatString("Handle VRML node <", context.node.header, ">."), LOGGING_INFO);

    auto resolvedAppearance =
        Traverse<ConversionContext>({nd->GetField<std::reference_wrapper<const VrmlNode>>("appearance").get(),
                                        context.manager, true, context.transformation, context.config},
            actionMap);
    if (resolvedAppearance.has_error()) {
      return cpp::fail(resolvedAppearance.error());
    }

    auto resolvedGeometry =
        Traverse<ConversionContext>({nd->GetField<std::reference_wrapper<const VrmlNode>>("geometry").get(),
                                        context.manager, true, context.transformation, context.config},
            actionMap);
    if (resolvedGeometry.has_error()) {
      return cpp::fail(resolvedGeometry.error());
    }

    nd->SetShapeDescendant(context.IsDescendantOfShape);
    nd->SetTransformationMatrix(context.transformation);
    auto data = HandlerToActionBundle<ConversionContext>(nd);
    data.cc1 = resolvedAppearance.value();
    data.cc2 = resolvedGeometry.value();
    data.config = context.config;

    return vrml_proc::traversor::utils::ConversionContextActionExecutor::TryToExecute<ConversionContext>(
        actionMap, nd->GetId(), data);
  }
}  // namespace vrml_proc::traversor::handler::ShapeHandler