#pragma once

#include <memory>
#include <functional>

#include <result.hpp>

#include "ConversionContextable.hpp"
#include "ConversionContextActionExecutor.hpp"
#include "ConversionContextActionMap.hpp"
#include "Error.hpp"
#include "FormatString.hpp"
#include "HandlerParameters.hpp"
#include "HandlerToActionBundle.hpp"
#include "Logger.hpp"
#include "NodeTraversorError.hpp"
#include "Transformation.hpp"
#include "TransformationMatrix.hpp"
#include "TraversorResult.hpp"
#include "Vec3f.hpp"
#include "VrmlNodeTraversorParameters.hpp"
#include "NodeView.hpp"

// Forward declaration.
namespace vrml_proc::traversor {
  template <vrml_proc::core::contract::ConversionContextable ConversionContext>
  class VrmlNodeTraversor;
}  // namespace vrml_proc::traversor

namespace vrml_proc::traversor::handler::TransformHandler {
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
  template <vrml_proc::core::contract::ConversionContextable ConversionContext>
  TraversorResult<ConversionContext> Handle(HandlerParameters<ConversionContext> params) {  //

    using namespace vrml_proc::core::logger;
    using namespace vrml_proc::core::utils;
    using namespace vrml_proc::math;
    using namespace vrml_proc::parser::model;
    using namespace vrml_proc::traversor::utils;

    // ---------------------------------------------------

    LogDebug(FormatString("Handle VRML node <", params.nodeView->GetName(), ">."), LOGGING_INFO);

    /** Update transformation data via copying. */
    Transformation transformationData;

    transformationData.center = params.nodeView->template GetField<std::reference_wrapper<const Vec3f>>("center").get();
    transformationData.rotation =
        params.nodeView->template GetField<std::reference_wrapper<const Vec4f>>("rotation").get();
    transformationData.scale = params.nodeView->template GetField<std::reference_wrapper<const Vec3f>>("scale").get();
    transformationData.scaleOrientation =
        params.nodeView->template GetField<std::reference_wrapper<const Vec4f>>("scaleOrientation").get();
    transformationData.translation =
        params.nodeView->template GetField<std::reference_wrapper<const Vec3f>>("translation").get();

    TransformationMatrix transformation = UpdateTransformationMatrix(params.transformation, transformationData);
    VrmlNodeTraversor<ConversionContext> traversor(params.manager, params.config, params.actionMap, params.headersMap);

    std::vector<std::shared_ptr<ConversionContext>> resolvedChildren;
    for (const auto& child :
        params.nodeView->template GetField<std::vector<std::reference_wrapper<const VrmlNode>>>("children")) {
      auto traversorParams = VrmlNodeTraversorParameters(child, params.IsDescendantOfShape, transformation);
      auto recursiveResult = traversor.Traverse(traversorParams);
      if (recursiveResult.has_error()) {
        return cpp::fail(recursiveResult.error());
      }
      resolvedChildren.push_back(recursiveResult.value());
    }

    // ---------------------------------------------------

    params.nodeView->SetShapeDescendant(params.IsDescendantOfShape);
    params.nodeView->SetTransformationMatrix(transformation);
    auto data = HandlerToActionBundle<ConversionContext>(params.nodeView);
    data.ccGroup = resolvedChildren;
    data.config = params.config;

    return ConversionContextActionExecutor::TryToExecute<ConversionContext>(
        params.actionMap, params.nodeView->GetName(), data);
  }
}  // namespace vrml_proc::traversor::handler::TransformHandler