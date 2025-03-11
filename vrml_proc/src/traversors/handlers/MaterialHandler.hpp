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
#include "Vec3f.hpp"
#include "VrmlUnits.hpp"

#include "VrmlProcessingExport.hpp"

namespace vrml_proc::traversor::handler::MaterialHandler {
  template <typename ConversionContext>
  VRMLPROCESSING_API inline cpp::result<std::shared_ptr<ConversionContext>,
                                        std::shared_ptr<vrml_proc::core::error::Error>>
  Handle(vrml_proc::traversor::VrmlNodeTraversorParameters context,
         const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap,
         const vrml_proc::traversor::node_descriptor::NodeDescriptor& nd) {
    vrml_proc::core::logger::LogInfo(
        vrml_proc::core::utils::FormatString("Handle VRML node <", context.node.header, ">."), LOGGING_INFO);

    std::any cachedAmbientIntensity =
        nd.GetField<std::reference_wrapper<const vrml_proc::parser::float32_t>>("ambientIntensity");

    std::any cachedDiffuseColor = nd.GetField<std::reference_wrapper<const vrml_proc::parser::Vec3f>>("diffuseColor");

    std::any cachedEmissiveColor = nd.GetField<std::reference_wrapper<const vrml_proc::parser::Vec3f>>("emissiveColor");

    std::any cachedShininess = nd.GetField<std::reference_wrapper<const vrml_proc::parser::float32_t>>("shininess");

    std::any cachedSpecularColor = nd.GetField<std::reference_wrapper<const vrml_proc::parser::Vec3f>>("specularColor");

    std::any cachedTransparency =
        nd.GetField<std::reference_wrapper<const vrml_proc::parser::float32_t>>("transparency");

    return vrml_proc::traversor::utils::ConversionContextActionExecutor::TryToExecute<ConversionContext>(
        actionMap, nd.GetId(),
        {std::cref(cachedAmbientIntensity), std::cref(cachedDiffuseColor), std::cref(cachedEmissiveColor),
         std::cref(cachedShininess), std::cref(cachedSpecularColor), std::cref(cachedTransparency)},
        {});
  }
}  // namespace vrml_proc::traversor::handler::MaterialHandler