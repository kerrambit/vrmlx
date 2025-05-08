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
#include "NodeTraversorError.hpp"
#include "Transformation.hpp"
#include "TransformationMatrix.hpp"
#include "TraversorResult.hpp"
#include "Vec3f.hpp"
#include "VrmlNodeTraversorParameters.hpp"

// Forward declaration.
namespace vrml_proc::traversor::VrmlNodeTraversor {
  template <ConversionContextable ConversionContext>
  cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> Traverse(
      vrml_proc::traversor::VrmlNodeTraversorParameters context,
      const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap);
}

namespace vrml_proc::traversor::handler::TransformHandler {
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

    using namespace vrml_proc::core::logger;
    using namespace vrml_proc::core::utils;
    using namespace vrml_proc::math;
    using namespace vrml_proc::parser;
    using namespace vrml_proc::traversor::utils;

    LogDebug(FormatString("Handle VRML node <", params.node.header, ">."), LOGGING_INFO);

    /** Update transformation data via copying. */
    Transformation transformationData;
    transformationData.center = nd->GetField<std::reference_wrapper<const Vec3f>>("center").get();
    transformationData.rotation = nd->GetField<std::reference_wrapper<const Vec4f>>("rotation").get();
    transformationData.scale = nd->GetField<std::reference_wrapper<const Vec3f>>("scale").get();
    transformationData.scaleOrientation = nd->GetField<std::reference_wrapper<const Vec4f>>("scaleOrientation").get();
    transformationData.translation = nd->GetField<std::reference_wrapper<const Vec3f>>("translation").get();

    TransformationMatrix transformation = UpdateTransformationMatrix(params.transformation, transformationData);

    std::vector<std::shared_ptr<ConversionContext>> resolvedChildren;
    for (const auto& child : nd->GetField<std::vector<std::reference_wrapper<const VrmlNode>>>("children")) {
      auto recursiveResult = vrml_proc::traversor::VrmlNodeTraversor::Traverse<ConversionContext>(
          {child, params.manager, params.IsDescendantOfShape, transformation, params.config}, actionMap);
      if (recursiveResult.has_error()) {
        return cpp::fail(recursiveResult.error());
      }
      resolvedChildren.push_back(recursiveResult.value());
    }

    nd->SetShapeDescendant(params.IsDescendantOfShape);
    nd->SetTransformationMatrix(transformation);
    auto data = HandlerToActionBundle<ConversionContext>(nd);
    data.ccGroup = resolvedChildren;
    data.config = params.config;

    return ConversionContextActionExecutor::TryToExecute<ConversionContext>(actionMap, nd->GetName(), data);
  }
}  // namespace vrml_proc::traversor::handler::TransformHandler