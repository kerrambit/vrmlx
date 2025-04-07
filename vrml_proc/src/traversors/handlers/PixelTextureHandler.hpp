#pragma once

#include <memory>

#include <any>
#include <result.hpp>

#include "ConversionContextActionExecutor.hpp"
#include "ConversionContextActionMap.hpp"
#include "Error.hpp"
#include "FormatString.hpp"
#include "VrmlNodeTraversorParameters.hpp"
#include "Logger.hpp"
#include "NodeTraversorError.hpp"
#include "Vec3fArray.hpp"
#include "HandlerResult.hpp"
#include "HandlerToActionBundle.hpp"

namespace vrml_proc::traversor::handler::PixelTextureHandler {
  template <typename ConversionContext>
  HandlerResult<ConversionContext> Handle(vrml_proc::traversor::VrmlNodeTraversorParameters context,
      const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap,
      std::shared_ptr<vrml_proc::traversor::node_descriptor::NodeView> nd) {  //

    vrml_proc::core::logger::LogInfo(
        vrml_proc::core::utils::FormatString("Handle VRML node <", context.node.header, ">."), LOGGING_INFO);

    nd->SetShapeDescendant(context.IsDescendantOfShape);
    nd->SetTransformationMatrix(context.transformation);
    auto data = HandlerToActionBundle<ConversionContext>(nd);

    return vrml_proc::traversor::utils::ConversionContextActionExecutor::TryToExecute<ConversionContext>(
        actionMap, nd->GetId(), data);
  }
}  // namespace vrml_proc::traversor::handler::PixelTextureHandler