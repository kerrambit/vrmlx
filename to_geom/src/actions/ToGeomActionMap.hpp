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

    actionMap.AddAction(
        "Box", [](const ConversionContextActionMap<MeshTaskConversionContext>::ReferencedArguments& refArgs,
                   const ConversionContextActionMap<MeshTaskConversionContext>::CopiedArguments& copyArgs) {
          try {
            return std::make_shared<BoxAction>(
                BoxAction::Properties{std::any_cast<std::reference_wrapper<const Vec3f>>(refArgs[0])},
                GeometryAction::Properties{
                    std::any_cast<bool>(copyArgs[0]), std::any_cast<TransformationMatrix>(copyArgs[1])});
          } catch (const std::bad_any_cast& e) {
            LogFatal("Invalid arguments for BoxAction!", LOGGING_INFO);
            throw;
          }
        });

    actionMap.AddAction(
        "Group", [](const ConversionContextActionMap<MeshTaskConversionContext>::ReferencedArguments& refArgs,
                     const ConversionContextActionMap<MeshTaskConversionContext>::CopiedArguments& copyArgs) {
          try {
            auto children = std::any_cast<std::vector<std::shared_ptr<MeshTaskConversionContext>>>(copyArgs[0]);
            auto bboxCenter = std::any_cast<std::reference_wrapper<const Vec3f>>(refArgs[0]);
            auto bboxSize = std::any_cast<std::reference_wrapper<const Vec3f>>(refArgs[1]);

            return std::make_shared<GroupAction>(GroupAction::Properties{children, bboxCenter, bboxSize});
          } catch (const std::bad_any_cast& e) {
            LogFatal("Invalid arguments for GroupAction!", LOGGING_INFO);
            throw;
          }
        });

    actionMap.AddAction(
        "Transform", [](const ConversionContextActionMap<MeshTaskConversionContext>::ReferencedArguments& refArgs,
                         const ConversionContextActionMap<MeshTaskConversionContext>::CopiedArguments& copyArgs) {
          try {
            TransformAction::Properties properties{std::any_cast<std::reference_wrapper<const Vec3f>>(refArgs[0]),
                std::any_cast<std::reference_wrapper<const Vec4f>>(refArgs[1]),
                std::any_cast<std::reference_wrapper<const Vec3f>>(refArgs[2]),
                std::any_cast<std::reference_wrapper<const Vec4f>>(refArgs[3]),
                std::any_cast<std::reference_wrapper<const Vec3f>>(refArgs[4]),
                std::any_cast<std::reference_wrapper<const Vec3f>>(refArgs[5]),
                std::any_cast<std::reference_wrapper<const Vec3f>>(refArgs[6]),
                std::any_cast<std::vector<std::shared_ptr<MeshTaskConversionContext>>>(copyArgs[0])};

            return std::make_shared<TransformAction>(properties);
          } catch (const std::bad_any_cast& e) {
            LogFatal("Invalid arguments for TransformAction!", LOGGING_INFO);
            throw;
          }
        });

    actionMap.AddAction(
        "Switch", [](const ConversionContextActionMap<MeshTaskConversionContext>::ReferencedArguments& refArgs,
                      const ConversionContextActionMap<MeshTaskConversionContext>::CopiedArguments& copyArgs) {
          try {
            SwitchAction::SwitchProperties properties{
                std::any_cast<std::shared_ptr<MeshTaskConversionContext>>(copyArgs[0])};
            return std::make_shared<SwitchAction>(properties);
          } catch (const std::bad_any_cast& e) {
            LogFatal("Invalid arguments for SwitchAction!", LOGGING_INFO);
            throw;
          }
        });

    actionMap.AddAction(
        "Shape", [](const ConversionContextActionMap<MeshTaskConversionContext>::ReferencedArguments& refArgs,
                     const ConversionContextActionMap<MeshTaskConversionContext>::CopiedArguments& copyArgs) {
          try {
            auto appearance = std::any_cast<std::shared_ptr<MeshTaskConversionContext>>(copyArgs[0]);
            auto geometry = std::any_cast<std::shared_ptr<MeshTaskConversionContext>>(copyArgs[1]);
            return std::make_shared<ShapeAction>(ShapeAction::Properties{appearance, geometry});
          } catch (const std::bad_any_cast& e) {
            LogFatal("Invalid arguments for ShapeAction!", LOGGING_INFO);
            throw;
          }
        });

    actionMap.AddAction(
        "IndexedFaceSet", [](const ConversionContextActionMap<MeshTaskConversionContext>::ReferencedArguments& refArgs,
                              const ConversionContextActionMap<MeshTaskConversionContext>::CopiedArguments& copyArgs) {
          try {
            IndexedFaceSetAction::Properties properties{
                std::any_cast<std::reference_wrapper<const VrmlNode>>(refArgs[0]),
                std::any_cast<std::reference_wrapper<const VrmlNode>>(refArgs[1]),
                std::any_cast<std::reference_wrapper<const VrmlNode>>(refArgs[2]),
                std::any_cast<std::reference_wrapper<const VrmlNode>>(refArgs[3]),
                std::any_cast<std::reference_wrapper<const bool>>(refArgs[4]),
                std::any_cast<std::reference_wrapper<const Int32Array>>(refArgs[5]),
                std::any_cast<std::reference_wrapper<const bool>>(refArgs[6]),
                std::any_cast<std::reference_wrapper<const bool>>(refArgs[7]),
                std::any_cast<std::reference_wrapper<const Int32Array>>(refArgs[8]),
                std::any_cast<std::reference_wrapper<const float32_t>>(refArgs[9]),
                std::any_cast<std::reference_wrapper<const Int32Array>>(refArgs[10]),
                std::any_cast<std::reference_wrapper<const bool>>(refArgs[11]),
                std::any_cast<std::reference_wrapper<const bool>>(refArgs[12]),
                std::any_cast<std::reference_wrapper<const Int32Array>>(refArgs[13])};

            return std::make_shared<IndexedFaceSetAction>(
                properties, GeometryAction::Properties{
                                std::any_cast<bool>(copyArgs[0]), std::any_cast<TransformationMatrix>(copyArgs[1])});
          } catch (const std::bad_any_cast& e) {
            LogFatal("Invalid arguments for IndexedFaceSet!", LOGGING_INFO);
            throw;
          }
        });

    actionMap.AddAction(
        "IndexedLineSet", [](const ConversionContextActionMap<MeshTaskConversionContext>::ReferencedArguments& refArgs,
                              const ConversionContextActionMap<MeshTaskConversionContext>::CopiedArguments& copyArgs) {
          try {
            to_geom::action::IndexedLineSetAction::Properties properties{
                std::any_cast<std::reference_wrapper<const VrmlNode>>(refArgs[0]),
                std::any_cast<std::reference_wrapper<const VrmlNode>>(refArgs[1]),
                std::any_cast<std::reference_wrapper<const Int32Array>>(refArgs[2]),
                std::any_cast<std::reference_wrapper<const bool>>(refArgs[3]),
                std::any_cast<std::reference_wrapper<const Int32Array>>(refArgs[4])};

            return std::make_shared<IndexedLineSetAction>(
                properties, GeometryAction::Properties{
                                std::any_cast<bool>(copyArgs[0]), std::any_cast<TransformationMatrix>(copyArgs[1])});
          } catch (const std::bad_any_cast& e) {
            LogFatal("Invalid arguments for IndexedLineSetAction!", LOGGING_INFO);
            throw;
          }
        });

    return actionMap;
  }
}  // namespace to_geom::conversion_context
