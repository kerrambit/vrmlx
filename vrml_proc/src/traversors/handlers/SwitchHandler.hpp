#pragma once

#include <cstdint>
#include <memory>

#include <result.hpp>

#include "ConversionContextActionExecutor.hpp"
#include "ConversionContextActionMap.hpp"
#include "Error.hpp"
#include "FormatString.hpp"
#include "VrmlNodeTraversorParameters.hpp"
#include "Logger.hpp"
#include "NodeTraversorError.hpp"
#include "NodeDescriptor.hpp"
#include "TraversorResult.hpp"
#include "HandlerToActionBundle.hpp"
#include "ConversionContextable.hpp"

namespace vrml_proc {
  namespace traversor {
    namespace VrmlNodeTraversor {
      template <ConversionContextable ConversionContext>
      VRMLPROCESSING_API cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>>
      Traverse(vrml_proc::traversor::VrmlNodeTraversorParameters context,
          const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap);
    }
  }  // namespace traversor
}  // namespace vrml_proc

#include "VrmlProcessingExport.hpp"

namespace vrml_proc::traversor::handler::SwitchHandler {

  template <ConversionContextable ConversionContext>
  TraversorResult<ConversionContext> Handle(vrml_proc::traversor::VrmlNodeTraversorParameters context,
      const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap,
      std::shared_ptr<vrml_proc::traversor::node_descriptor::NodeView> nd) {  //

    using namespace vrml_proc::traversor::VrmlNodeTraversor;
    using vrml_proc::parser::VrmlNode;

    vrml_proc::core::logger::LogInfo(
        vrml_proc::core::utils::FormatString("Handle VRML node <", context.node.header, ">."), LOGGING_INFO);

    const int32_t& whichChoice = (nd->GetField<std::reference_wrapper<const int32_t>>("whichChoice")).get();
    std::shared_ptr<ConversionContext> resolvedChild = std::make_shared<ConversionContext>();

    if (whichChoice >= 0 && nd->GetField<std::vector<std::reference_wrapper<const VrmlNode>>>("choice").size() != 0 &&
        whichChoice <= nd->GetField<std::vector<std::reference_wrapper<const VrmlNode>>>("choice").size() - 1) {
      auto recursiveResult = Traverse<ConversionContext>(
          {nd->GetField<std::vector<std::reference_wrapper<const VrmlNode>>>("choice").at(whichChoice), context.manager,
              context.IsDescendantOfShape, context.transformation, context.config},
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

    nd->SetShapeDescendant(context.IsDescendantOfShape);
    nd->SetTransformationMatrix(context.transformation);
    auto data = HandlerToActionBundle<ConversionContext>(nd);
    data.cc1 = resolvedChild;
    data.config = context.config;

    return vrml_proc::traversor::utils::ConversionContextActionExecutor::TryToExecute<ConversionContext>(
        actionMap, nd->GetId(), data);
  }
}  // namespace vrml_proc::traversor::handler::SwitchHandler