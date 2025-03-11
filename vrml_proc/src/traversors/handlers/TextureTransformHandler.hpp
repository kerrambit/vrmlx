#pragma once

#include <any>
#include <result.hpp>

#include "ConversionContextActionExecutor.hpp"
#include "ConversionContextActionMap.hpp"
#include "Error.hpp"
#include "FormatString.hpp"
#include "VrmlNodeTraversorParameters.hpp"
#include "Logger.hpp"
#include "NodeTraversorError.hpp"
#include "Vec2f.hpp"
#include "VrmlUnits.hpp"

#include "VrmlProcessingExport.hpp"

namespace vrml_proc::traversor::handler::TextureTransformHandler {
  template <typename ConversionContext>
  VRMLPROCESSING_API inline cpp::result<std::shared_ptr<ConversionContext>,
                                        std::shared_ptr<vrml_proc::core::error::Error>>
  Handle(vrml_proc::traversor::VrmlNodeTraversorParameters context,
         const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap,
         const vrml_proc::traversor::node_descriptor::NodeDescriptor& nd) {
    vrml_proc::core::logger::LogInfo(
        vrml_proc::core::utils::FormatString("Handle VRML node <", context.node.header, ">."), LOGGING_INFO);

    std::any cachedCenter = nd.GetField<std::reference_wrapper<const vrml_proc::parser::Vec2f>>("center");
    std::any cachedRotation = nd.GetField<std::reference_wrapper<const vrml_proc::parser::float32_t>>("rotation");
    std::any cachedScale = nd.GetField<std::reference_wrapper<const vrml_proc::parser::Vec2f>>("scale");
    std::any cachedTranslation = nd.GetField<std::reference_wrapper<const vrml_proc::parser::Vec2f>>("translation");

    return vrml_proc::traversor::utils::ConversionContextActionExecutor::TryToExecute<ConversionContext>(
        actionMap, nd.GetId(),
        {std::cref(cachedCenter), std::cref(cachedRotation), std::cref(cachedScale), std::cref(cachedTranslation)}, {});
  }
}  // namespace vrml_proc::traversor::handler::TextureTransformHandler