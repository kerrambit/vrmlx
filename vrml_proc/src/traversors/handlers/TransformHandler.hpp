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
#include "Transformation.hpp"
#include "TransformationMatrix.hpp"
#include "Vec3f.hpp"
#include "TraversorResult.hpp"
#include "HandlerToActionBundle.hpp"
#include "ConversionContextable.hpp"

// Forward declaration.
namespace vrml_proc::traversor::VrmlNodeTraversor {
  template <ConversionContextable ConversionContext>
  VRMLPROCESSING_API cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>>
  Traverse(vrml_proc::traversor::VrmlNodeTraversorParameters context,
      const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap);
}

namespace vrml_proc::traversor::handler::TransformHandler {

  template <ConversionContextable ConversionContext>
  TraversorResult<ConversionContext> Handle(vrml_proc::traversor::VrmlNodeTraversorParameters context,
      const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap,
      std::shared_ptr<vrml_proc::traversor::node_descriptor::NodeView> nd) {  //

    using namespace vrml_proc::core::logger;
    using namespace vrml_proc::core::utils;
    using namespace vrml_proc::math;
    using namespace vrml_proc::parser;
    using namespace vrml_proc::traversor::utils;

    LogInfo(FormatString("Handle VRML node <", context.node.header, ">."), LOGGING_INFO);

    /** Update transformation data via copying. */
    Transformation transformationData;
    transformationData.center = nd->GetField<std::reference_wrapper<const Vec3f>>("center").get();
    transformationData.rotation = nd->GetField<std::reference_wrapper<const Vec4f>>("rotation").get();
    transformationData.scale = nd->GetField<std::reference_wrapper<const Vec3f>>("scale").get();
    transformationData.scaleOrientation = nd->GetField<std::reference_wrapper<const Vec4f>>("scaleOrientation").get();
    transformationData.translation = nd->GetField<std::reference_wrapper<const Vec3f>>("translation").get();

    TransformationMatrix transformation = UpdateTransformationMatrix(context.transformation, transformationData);

    std::vector<std::shared_ptr<ConversionContext>> resolvedChildren;
    for (const auto& child : nd->GetField<std::vector<std::reference_wrapper<const VrmlNode>>>("children")) {
      auto recursiveResult = vrml_proc::traversor::VrmlNodeTraversor::Traverse<ConversionContext>(
          {child, context.manager, context.IsDescendantOfShape, transformation, context.config}, actionMap);
      if (recursiveResult.has_error()) {
        return cpp::fail(recursiveResult.error());
      }
      resolvedChildren.push_back(recursiveResult.value());
    }

    nd->SetShapeDescendant(context.IsDescendantOfShape);
    nd->SetTransformationMatrix(context.transformation);
    auto data = HandlerToActionBundle<ConversionContext>(nd);
    data.ccGroup = resolvedChildren;

    return ConversionContextActionExecutor::TryToExecute<ConversionContext>(actionMap, nd->GetId(), data);
  }
}  // namespace vrml_proc::traversor::handler::TransformHandler