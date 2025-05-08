#pragma once

#include <functional>
#include <memory>

#include "BoxAction.hpp"
#include "ConversionContextActionMap.hpp"
#include "GroupAction.hpp"
#include "HandlerToActionBundle.hpp"
#include "IndexedFaceSetAction.hpp"
#include "IndexedLineSetAction.hpp"
#include "MeshTaskConversionContext.hpp"
#include "ShapeAction.hpp"
#include "SwitchAction.hpp"
#include "ToGeomConfig.hpp"
#include "TransformAction.hpp"
#include "Vec3f.hpp"
#include "VrmlNode.hpp"

namespace to_geom::conversion_context {
  /**
   * @brief Retrieves action map. Action map maps VRML node name to an corresponding actions. Or rather, it maps it to a
   * lambda which will create given action class. This given returns a map which is basically a cookbook to create an
   * individual action instance.
   *
   * @returns action map with MeshTaskConversionContext as conversion context
   */
  inline vrml_proc::action::ConversionContextActionMap<to_geom::conversion_context::MeshTaskConversionContext>&
  GetActionMap() {  //

    using to_geom::conversion_context::MeshTaskConversionContext;
    using vrml_proc::action::ConversionContextActionMap;
    using vrml_proc::math::TransformationMatrix;
    using namespace vrml_proc::parser;
    using namespace to_geom::action;
    using namespace vrml_proc::core::logger;

    static ConversionContextActionMap<MeshTaskConversionContext> actionMap;
    static bool initialized = false;

    if (initialized) {
      return actionMap;
    }

    if (!initialized) {
      initialized = true;
    }

    actionMap.AddAction(
        "Box", [](vrml_proc::traversor::handler::HandlerToActionBundle<MeshTaskConversionContext> data) {
          return std::make_shared<BoxAction>(
              BoxAction::Properties{data.nodeView->GetField<std::reference_wrapper<const Vec3f>>("size")},
              GeometryAction::Properties{
                  data.nodeView->IsNodeShapeDescendant(), data.nodeView->GetTransformationMatrix()});
        });

    actionMap.AddAction(
        "Group", [](vrml_proc::traversor::handler::HandlerToActionBundle<MeshTaskConversionContext> data) {
          return std::make_shared<GroupAction>(GroupAction::Properties{data.ccGroup});
        });

    actionMap.AddAction(
        "Transform", [](vrml_proc::traversor::handler::HandlerToActionBundle<MeshTaskConversionContext> data) {
          return std::make_shared<TransformAction>(TransformAction::Properties{data.ccGroup});
        });

    actionMap.AddAction(
        "Switch", [](vrml_proc::traversor::handler::HandlerToActionBundle<MeshTaskConversionContext> data) {
          return std::make_shared<SwitchAction>(SwitchAction::SwitchProperties{data.cc1});
        });

    actionMap.AddAction(
        "Shape", [](vrml_proc::traversor::handler::HandlerToActionBundle<MeshTaskConversionContext> data) {
          return std::make_shared<ShapeAction>(ShapeAction::Properties{data.cc1, data.cc2});
        });

    actionMap.AddAction(
        "IndexedFaceSet", [](vrml_proc::traversor::handler::HandlerToActionBundle<MeshTaskConversionContext> data) {
          auto coord = data.nodeView->GetField<std::reference_wrapper<const VrmlNode>>("coord");
          auto coordIndex = data.nodeView->GetField<std::reference_wrapper<const Int32Array>>("coordIndex");
          auto convex = data.nodeView->GetField<std::reference_wrapper<const bool>>("convex");
          auto geomConfig = std::static_pointer_cast<to_geom::core::config::ToGeomConfig>(data.config);

          IndexedFaceSetAction::Properties properties{coord, convex, coordIndex, geomConfig};

          return std::make_shared<IndexedFaceSetAction>(
              properties, GeometryAction::Properties{
                              data.nodeView->IsNodeShapeDescendant(), data.nodeView->GetTransformationMatrix()});
        });

    actionMap.AddAction(
        "IndexedLineSet", [](vrml_proc::traversor::handler::HandlerToActionBundle<MeshTaskConversionContext> data) {
          auto coord = data.nodeView->GetField<std::reference_wrapper<const VrmlNode>>("coord");
          auto coordIndex = data.nodeView->GetField<std::reference_wrapper<const Int32Array>>("coordIndex");
          return std::make_shared<IndexedLineSetAction>(IndexedLineSetAction::Properties{coord, coordIndex},
              GeometryAction::Properties{
                  data.nodeView->IsNodeShapeDescendant(), data.nodeView->GetTransformationMatrix()});
        });

    return actionMap;
  }
}  // namespace to_geom::conversion_context
