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

#include "VrmlProcessingExport.hpp"

namespace vrml_proc::traversor::handler::ImageTextureHandler {
  template <typename ConversionContext>
  VRMLPROCESSING_API inline cpp::result<std::shared_ptr<ConversionContext>,
                                        std::shared_ptr<vrml_proc::core::error::Error>>
  Handle(vrml_proc::traversor::VrmlNodeTraversorParameters context,
         const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap,
         const vrml_proc::traversor::node_descriptor::NodeDescriptor& nd) {
    vrml_proc::core::logger::LogInfo(
        vrml_proc::core::utils::FormatString("Handle VRML node <", context.node.header, ">."), LOGGING_INFO);

    std::any cachedUrl = nd.GetField<std::reference_wrapper<const std::string>>("url");
    std::any cachedRepeatS = nd.GetField<std::reference_wrapper<const bool>>("repeatS");
    std::any cachedRepeatT = nd.GetField<std::reference_wrapper<const bool>>("repeatT");

    return vrml_proc::traversor::utils::ConversionContextActionExecutor::TryToExecute<ConversionContext>(
        actionMap, nd.GetId(), {std::cref(cachedUrl), std::cref(cachedRepeatS), std::cref(cachedRepeatT)}, {});
  }
}  // namespace vrml_proc::traversor::handler::ImageTextureHandler