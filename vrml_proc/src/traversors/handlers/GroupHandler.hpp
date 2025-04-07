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
#include "Vec3f.hpp"
#include "NodeDescriptor.hpp"
#include "HandlerResult.hpp"
#include "HandlerToActionBundle.hpp"

#include "VrmlProcessingExport.hpp"

// Forward declaration.
namespace vrml_proc::traversor::VrmlNodeTraversor {
  template <typename ConversionContext>
  VRMLPROCESSING_API cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>>
  Traverse(vrml_proc::traversor::VrmlNodeTraversorParameters context,
      const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap);
}

namespace vrml_proc::traversor::handler::GroupHandler {

  template <typename ConversionContext>
  HandlerResult<ConversionContext> Handle(vrml_proc::traversor::VrmlNodeTraversorParameters context,
      const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap,
      std::shared_ptr<vrml_proc::traversor::node_descriptor::NodeView> nd) {  //

    using namespace vrml_proc::traversor::VrmlNodeTraversor;
    using vrml_proc::parser::VrmlNode;

    vrml_proc::core::logger::LogInfo(
        vrml_proc::core::utils::FormatString("Handle VRML node <", context.node.header, ">."), LOGGING_INFO);

    std::vector<std::shared_ptr<ConversionContext>> resolvedChildren;
    for (const auto& child : nd->GetField<std::vector<std::reference_wrapper<const VrmlNode>>>("children")) {
      auto recursiveResult = Traverse<ConversionContext>(
          {child, context.manager, context.IsDescendantOfShape, context.transformation, context.config}, actionMap);
      if (recursiveResult.has_error()) {
        return cpp::fail(recursiveResult.error());
      }
      resolvedChildren.push_back(recursiveResult.value());
    }

    nd->SetShapeDescendant(context.IsDescendantOfShape);
    nd->SetTransformationMatrix(context.transformation);
    auto data = HandlerToActionBundle<ConversionContext>(nd);
    data.ccGroup = resolvedChildren;

    return vrml_proc::traversor::utils::ConversionContextActionExecutor::TryToExecute<ConversionContext>(
        actionMap, nd->GetId(), data);
  }
}  // namespace vrml_proc::traversor::handler::GroupHandler