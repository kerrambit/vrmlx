#pragma once

#include <any>
#include <functional>
#include <memory>

#include "BoxAction.hpp"
#include "ConversionContextActionMap.hpp"
#include "GroupAction.hpp"
#include "IndexedFaceSetAction.hpp"
#include "IndexedLineSetAction.hpp"
#include "MeshTaskConversionContext.hpp"
#include "ShapeAction.hpp"
#include "SwitchAction.hpp"
#include "TransformAction.hpp"
#include "Vec3f.hpp"
#include "NodeView.hpp"
#include "HandlerToActionBundle.hpp"

namespace to_geom::conversion_context {

  /**
   * @brief Gets an action map. Action map maps VRML node name to an corresponding actions. Or rather, it maps it to a
   * lambda which will create given action class.
   *
   * @returns action map with MeshTaskConversionContext as conversion context
   */
  inline vrml_proc::action::ConversionContextActionMap<to_geom::conversion_context::MeshTaskConversionContext>&
  GetActionMap() {
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

    actionMap.AddActionForHandlerToActionBundle(
        "Box", [](vrml_proc::traversor::handler::HandlerToActionBundle<MeshTaskConversionContext> data) {
          try {
            return std::make_shared<BoxAction>(
                BoxAction::Properties{
                    data.nodeView->GetField<std::reference_wrapper<const vrml_proc::parser::Vec3f>>("size")},
                GeometryAction::Properties{
                    data.nodeView->IsNodeShapeDescendant(), data.nodeView->GetTransformationMatrix()});
          } catch (const std::bad_any_cast& e) {
            LogFatal("Invalid arguments for BoxAction!", LOGGING_INFO);
            throw;
          }
        });

    actionMap.AddActionForHandlerToActionBundle(
        "Group", [](vrml_proc::traversor::handler::HandlerToActionBundle<MeshTaskConversionContext> data) {
          try {
            return std::make_shared<GroupAction>(GroupAction::Properties{data.ccGroup,
                data.nodeView->GetField<std::reference_wrapper<const vrml_proc::parser::Vec3f>>("bboxCenter"),
                data.nodeView->GetField<std::reference_wrapper<const vrml_proc::parser::Vec3f>>("bboxSize")});
          } catch (const std::bad_any_cast& e) {
            LogFatal("Invalid arguments for GroupAction!", LOGGING_INFO);
            throw;
          }
        });

    actionMap.AddActionForHandlerToActionBundle(
        "Transform", [](vrml_proc::traversor::handler::HandlerToActionBundle<MeshTaskConversionContext> data) {
          try {
            TransformAction::Properties properties{data.ccGroup};

            return std::make_shared<TransformAction>(properties);
          } catch (const std::bad_any_cast& e) {
            LogFatal("Invalid arguments for TransformAction!", LOGGING_INFO);
            throw;
          }
        });

    actionMap.AddActionForHandlerToActionBundle(
        "Switch", [](vrml_proc::traversor::handler::HandlerToActionBundle<MeshTaskConversionContext> data) {
          try {
            SwitchAction::SwitchProperties properties{data.cc1};
            return std::make_shared<SwitchAction>(properties);
          } catch (const std::bad_any_cast& e) {
            LogFatal("Invalid arguments for SwitchAction!", LOGGING_INFO);
            throw;
          }
        });

    actionMap.AddActionForHandlerToActionBundle(
        "Shape", [](vrml_proc::traversor::handler::HandlerToActionBundle<MeshTaskConversionContext> data) {
          try {
            return std::make_shared<ShapeAction>(ShapeAction::Properties{data.cc1, data.cc2});
          } catch (const std::bad_any_cast& e) {
            LogFatal("Invalid arguments for ShapeAction!", LOGGING_INFO);
            throw;
          }
        });

    actionMap.AddActionForHandlerToActionBundle(
        "IndexedFaceSet", [](vrml_proc::traversor::handler::HandlerToActionBundle<MeshTaskConversionContext> data) {
          try {
            auto coord = data.nodeView->GetField<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>("coord");
            auto coordIndex =
                data.nodeView->GetField<std::reference_wrapper<const vrml_proc::parser::Int32Array>>("coordIndex");
            auto convex = data.nodeView->GetField<std::reference_wrapper<const bool>>("convex");

            IndexedFaceSetAction::Properties properties{coord, convex, coordIndex};

            return std::make_shared<IndexedFaceSetAction>(
                properties, GeometryAction::Properties{
                                data.nodeView->IsNodeShapeDescendant(), data.nodeView->GetTransformationMatrix()});
          } catch (const std::bad_any_cast& e) {
            LogFatal("Invalid arguments for IndexedFaceSet!", LOGGING_INFO);
            throw;
          }
        });

    actionMap.AddActionForHandlerToActionBundle(
        "IndexedLineSet", [](vrml_proc::traversor::handler::HandlerToActionBundle<MeshTaskConversionContext> data) {
          try {
            auto coord = data.nodeView->GetField<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>("coord");
            auto coordIndex =
                data.nodeView->GetField<std::reference_wrapper<const vrml_proc::parser::Int32Array>>("coordIndex");

            to_geom::action::IndexedLineSetAction::Properties properties{coord, coordIndex};

            return std::make_shared<IndexedLineSetAction>(
                properties, GeometryAction::Properties{
                                data.nodeView->IsNodeShapeDescendant(), data.nodeView->GetTransformationMatrix()});
          } catch (const std::bad_any_cast& e) {
            LogFatal("Invalid arguments for IndexedLineSetAction!", LOGGING_INFO);
            throw;
          }
        });

    return actionMap;
  }
}  // namespace to_geom::conversion_context
