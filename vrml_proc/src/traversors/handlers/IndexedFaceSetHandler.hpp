#pragma once

#include <any>
#include <memory>

#include <result.hpp>

#include "ConversionContextActionExecutor.hpp"
#include "ConversionContextActionMap.hpp"
#include "Error.hpp"
#include "FormatString.hpp"
#include "VrmlNodeTraversorParameters.hpp"
#include "Int32Array.hpp"
#include "Logger.hpp"
#include "NodeTraversorError.hpp"
#include "VrmlNode.hpp"
#include "VrmlUnits.hpp"

#include "VrmlProcessingExport.hpp"

namespace vrml_proc::traversor::handler::IndexedFaceSetHandler {

  template <typename ConversionContext>
  VRMLPROCESSING_API cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>>
  Handle(vrml_proc::traversor::VrmlNodeTraversorParameters context,
         const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap,
         const vrml_proc::traversor::node_descriptor::NodeDescriptor& nd) {
    vrml_proc::core::logger::LogInfo(
        vrml_proc::core::utils::FormatString("Handle VRML node <", context.node.header, ">."), LOGGING_INFO);

    // There are geometry primitive nodes. They are send as VrmlNodes into the given action where they can be traversed
    // if needed.
    // TODO: we should run validators for coord, color, normal and texCoord

    std::any color = nd.GetField<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>("color");
    std::any coord = nd.GetField<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>("coord");
    std::any normal = nd.GetField<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>("normal");
    std::any texCoord = nd.GetField<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>("texCoord");
    std::any ccw = nd.GetField<std::reference_wrapper<const bool>>("ccw");
    std::any colorIndex = nd.GetField<std::reference_wrapper<const vrml_proc::parser::Int32Array>>("colorIndex");
    std::any colorPerVertex = nd.GetField<std::reference_wrapper<const bool>>("colorPerVertex");
    std::any convex = nd.GetField<std::reference_wrapper<const bool>>("convex");
    std::any coordIndex = nd.GetField<std::reference_wrapper<const vrml_proc::parser::Int32Array>>("coordIndex");
    std::any creaseAngle = nd.GetField<std::reference_wrapper<const vrml_proc::parser::float32_t>>("creaseAngle");
    std::any normalIndex = nd.GetField<std::reference_wrapper<const vrml_proc::parser::Int32Array>>("normalIndex");
    std::any normalPerVertex = nd.GetField<std::reference_wrapper<const bool>>("normalPerVertex");
    std::any solid = nd.GetField<std::reference_wrapper<const bool>>("solid");
    std::any texCoordIndex = nd.GetField<std::reference_wrapper<const vrml_proc::parser::Int32Array>>("texCoordIndex");

    return vrml_proc::traversor::utils::ConversionContextActionExecutor::TryToExecute<ConversionContext>(
        actionMap, nd.GetId(),
        {
            std::cref(color),
            std::cref(coord),
            std::cref(normal),
            std::cref(texCoord),
            std::cref(ccw),
            std::cref(colorIndex),
            std::cref(colorPerVertex),
            std::cref(convex),
            std::cref(coordIndex),
            std::cref(creaseAngle),
            std::cref(normalIndex),
            std::cref(normalPerVertex),
            std::cref(solid),
            std::cref(texCoordIndex),
        },
        {context.IsDescendantOfShape, context.transformation});
  }
}  // namespace vrml_proc::traversor::handler::IndexedFaceSetHandler