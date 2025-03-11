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

#include "VrmlProcessingExport.hpp"

// Forward declaration.
namespace vrml_proc::traversor::VrmlNodeTraversor {
  template <typename ConversionContext>
  VRMLPROCESSING_API cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>>
  Traverse(vrml_proc::traversor::VrmlNodeTraversorParameters context,
           const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap);
}

namespace vrml_proc::traversor::handler::AppearanceHandler {

  template <typename ConversionContext>
  VRMLPROCESSING_API inline cpp::result<std::shared_ptr<ConversionContext>,
                                        std::shared_ptr<vrml_proc::core::error::Error>>
  Handle(vrml_proc::traversor::VrmlNodeTraversorParameters context,
         const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap,
         const vrml_proc::traversor::node_descriptor::NodeDescriptor& nd) {
    vrml_proc::core::logger::LogInfo(
        vrml_proc::core::utils::FormatString("Handle VRML node <", context.node.header, ">."), LOGGING_INFO);

    auto resolvedMaterial = vrml_proc::traversor::VrmlNodeTraversor::Traverse<ConversionContext>(
        {nd.GetField<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>("material").get(), context.manager,
         context.IsDescendantOfShape, context.transformation, context.config},
        actionMap);
    if (resolvedMaterial.has_error()) {
      return cpp::fail(resolvedMaterial.error());
    }

    auto resolvedTexture = vrml_proc::traversor::VrmlNodeTraversor::Traverse<ConversionContext>(
        {nd.GetField<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>("texture").get(), context.manager,
         context.IsDescendantOfShape, context.transformation, context.config},
        actionMap);
    if (resolvedTexture.has_error()) {
      return cpp::fail(resolvedTexture.error());
    }

    auto resolvedTextureTransform = vrml_proc::traversor::VrmlNodeTraversor::Traverse<ConversionContext>(
        {nd.GetField<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>("textureTransform").get(),
         context.manager, context.IsDescendantOfShape, context.transformation, context.config},
        actionMap);
    if (resolvedTextureTransform.has_error()) {
      return cpp::fail(resolvedTextureTransform.error());
    }

    return vrml_proc::traversor::utils::ConversionContextActionExecutor::TryToExecute<ConversionContext>(
        actionMap, nd.GetId(), {},
        {resolvedMaterial.value(), resolvedTexture.value(), resolvedTextureTransform.value()});
  }
}  // namespace vrml_proc::traversor::handler::AppearanceHandler