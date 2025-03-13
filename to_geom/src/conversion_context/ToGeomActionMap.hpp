#pragma once

#include <any>
#include <functional>
#include <memory>

#include "BoxAction.hpp"
#include "ConversionContextActionMap.hpp"
#include "GroupAction.hpp"
#include "IndexedFaceSetAction.hpp"
#include "MeshTaskConversionContext.hpp"
#include "ShapeAction.hpp"
#include "SwitchAction.hpp"
#include "IndexedLineSetAction.hpp"
#include "TransformAction.hpp"

namespace to_geom::conversion_context {

  inline vrml_proc::action::ConversionContextActionMap<to_geom::conversion_context::MeshTaskConversionContext>&
  CreateActionMap() {
    static vrml_proc::action::ConversionContextActionMap<to_geom::conversion_context::MeshTaskConversionContext>
        actionMap;
    static bool initialized = false;

    if (initialized) {
      return actionMap;
    }

    if (!initialized) {
      initialized = true;
    }

    actionMap.AddAction(
        "Box", [](const vrml_proc::action::ConversionContextActionMap<
                      to_geom::conversion_context::MeshTaskConversionContext>::ReferencedArguments& refArgs,

                  const vrml_proc::action::ConversionContextActionMap<
                      to_geom::conversion_context::MeshTaskConversionContext>::CopiedArguments& copyArgs) {
          if (refArgs.size() == 1 &&
              refArgs[0].get().type() == typeid(std::reference_wrapper<const vrml_proc::parser::Vec3f>) &&
              copyArgs.size() == 2 && copyArgs[0].type() == typeid(bool) &&
              copyArgs[1].type() == typeid(vrml_proc::math::TransformationMatrix)) {
            return std::make_shared<to_geom::action::BoxAction>(
                to_geom::action::BoxAction::Properties{
                    std::any_cast<std::reference_wrapper<const vrml_proc::parser::Vec3f>>(refArgs[0])},
                to_geom::action::GeometryAction::Properties{
                    std::any_cast<bool>(copyArgs[0]),
                    std::any_cast<vrml_proc::math::TransformationMatrix>(copyArgs[1])});
          }

          vrml_proc::core::logger::LogFatal("Invalid arguments for BoxAction!", LOGGING_INFO);
          throw std::runtime_error("Invalid arguments for BoxAction");
        });

    actionMap.AddAction(
        "Group", [](const vrml_proc::action::ConversionContextActionMap<
                        to_geom::conversion_context::MeshTaskConversionContext>::ReferencedArguments& refArgs,
                    const vrml_proc::action::ConversionContextActionMap<
                        to_geom::conversion_context::MeshTaskConversionContext>::CopiedArguments& copyArgs) {
          if (refArgs.size() == 2 &&
              refArgs[0].get().type() == typeid(std::reference_wrapper<const vrml_proc::parser::Vec3f>) &&
              refArgs[1].get().type() == typeid(std::reference_wrapper<const vrml_proc::parser::Vec3f>) &&
              copyArgs.size() == 1 &&
              copyArgs[0].type() ==
                  typeid(std::vector<std::shared_ptr<to_geom::conversion_context::MeshTaskConversionContext>>)) {
            auto children =
                std::any_cast<std::vector<std::shared_ptr<to_geom::conversion_context::MeshTaskConversionContext>>>(
                    copyArgs[0]);
            auto bboxCenter = std::any_cast<std::reference_wrapper<const vrml_proc::parser::Vec3f>>(refArgs[0]);
            auto bboxSize = std::any_cast<std::reference_wrapper<const vrml_proc::parser::Vec3f>>(refArgs[1]);

            return std::make_shared<to_geom::action::GroupAction>(children, bboxCenter, bboxSize);
          }

          vrml_proc::core::logger::LogFatal("Invalid arguments for GroupAction!", LOGGING_INFO);
          throw std::runtime_error("Invalid arguments for GroupAction");
        });

    actionMap.AddAction(
        "Transform", [](const vrml_proc::action::ConversionContextActionMap<
                            to_geom::conversion_context::MeshTaskConversionContext>::ReferencedArguments& refArgs,
                        const vrml_proc::action::ConversionContextActionMap<
                            to_geom::conversion_context::MeshTaskConversionContext>::CopiedArguments& copyArgs) {
          if (refArgs.size() == 7 &&
              refArgs[0].get().type() == typeid(std::reference_wrapper<const vrml_proc::parser::Vec3f>) &&
              refArgs[1].get().type() == typeid(std::reference_wrapper<const vrml_proc::parser::Vec4f>) &&
              refArgs[2].get().type() == typeid(std::reference_wrapper<const vrml_proc::parser::Vec3f>) &&
              refArgs[3].get().type() == typeid(std::reference_wrapper<const vrml_proc::parser::Vec4f>) &&
              refArgs[4].get().type() == typeid(std::reference_wrapper<const vrml_proc::parser::Vec3f>) &&
              refArgs[5].get().type() == typeid(std::reference_wrapper<const vrml_proc::parser::Vec3f>) &&
              refArgs[6].get().type() == typeid(std::reference_wrapper<const vrml_proc::parser::Vec3f>) &&
              copyArgs.size() == 1 &&
              copyArgs[0].type() ==
                  typeid(std::vector<std::shared_ptr<to_geom::conversion_context::MeshTaskConversionContext>>)) {
            to_geom::action::TransformAction::TransformProperties properties{
                std::any_cast<std::reference_wrapper<const vrml_proc::parser::Vec3f>>(refArgs[0]),
                std::any_cast<std::reference_wrapper<const vrml_proc::parser::Vec4f>>(refArgs[1]),
                std::any_cast<std::reference_wrapper<const vrml_proc::parser::Vec3f>>(refArgs[2]),
                std::any_cast<std::reference_wrapper<const vrml_proc::parser::Vec4f>>(refArgs[3]),
                std::any_cast<std::reference_wrapper<const vrml_proc::parser::Vec3f>>(refArgs[4]),
                std::any_cast<std::reference_wrapper<const vrml_proc::parser::Vec3f>>(refArgs[5]),
                std::any_cast<std::reference_wrapper<const vrml_proc::parser::Vec3f>>(refArgs[6]),
                std::any_cast<std::vector<std::shared_ptr<to_geom::conversion_context::MeshTaskConversionContext>>>(
                    copyArgs[0])};

            return std::make_shared<to_geom::action::TransformAction>(properties);
          }

          vrml_proc::core::logger::LogFatal("Invalid arguments for TransformAction!", LOGGING_INFO);
          throw std::runtime_error("Invalid arguments for TransformAction");
        });

    actionMap.AddAction(
        "Switch", [](const vrml_proc::action::ConversionContextActionMap<
                         to_geom::conversion_context::MeshTaskConversionContext>::ReferencedArguments& refArgs,
                     const vrml_proc::action::ConversionContextActionMap<
                         to_geom::conversion_context::MeshTaskConversionContext>::CopiedArguments& copyArgs) {
          if (refArgs.size() == 0 && copyArgs.size() == 1 &&
              copyArgs[0].type() == typeid(std::shared_ptr<to_geom::conversion_context::MeshTaskConversionContext>)) {
            to_geom::action::SwitchAction::SwitchProperties properties{
                std::any_cast<std::shared_ptr<to_geom::conversion_context::MeshTaskConversionContext>>(copyArgs[0])};

            return std::make_shared<to_geom::action::SwitchAction>(properties);
          }

          vrml_proc::core::logger::LogFatal("Invalid arguments for SwitchAction!", LOGGING_INFO);
          throw std::runtime_error("Invalid arguments for SwitchAction");
        });

    actionMap.AddAction(
        "Shape", [](const vrml_proc::action::ConversionContextActionMap<
                        to_geom::conversion_context::MeshTaskConversionContext>::ReferencedArguments& refArgs,
                    const vrml_proc::action::ConversionContextActionMap<
                        to_geom::conversion_context::MeshTaskConversionContext>::CopiedArguments& copyArgs) {
          if (refArgs.size() == 0 && copyArgs.size() == 2 &&
              copyArgs[0].type() == typeid(std::shared_ptr<to_geom::conversion_context::MeshTaskConversionContext>) &&
              copyArgs[1].type() == typeid(std::shared_ptr<to_geom::conversion_context::MeshTaskConversionContext>)) {
            auto appearance =
                std::any_cast<std::shared_ptr<to_geom::conversion_context::MeshTaskConversionContext>>(copyArgs[0]);
            auto geometry =
                std::any_cast<std::shared_ptr<to_geom::conversion_context::MeshTaskConversionContext>>(copyArgs[1]);

            return std::make_shared<to_geom::action::ShapeAction>(appearance, geometry);
          }

          vrml_proc::core::logger::LogFatal("Invalid arguments for ShapeAction!", LOGGING_INFO);
          throw std::runtime_error("Invalid arguments for ShapeAction");
        });

    actionMap.AddAction(
        "IndexedFaceSet", [](const vrml_proc::action::ConversionContextActionMap<
                                 to_geom::conversion_context::MeshTaskConversionContext>::ReferencedArguments& refArgs,
                             const vrml_proc::action::ConversionContextActionMap<
                                 to_geom::conversion_context::MeshTaskConversionContext>::CopiedArguments& copyArgs) {
          using vrml_proc::parser::VrmlNode;
          using vrml_proc::parser::Int32Array;
          using vrml_proc::parser::float32_t;

          if (refArgs.size() == 14 && copyArgs.size() == 2 && copyArgs[0].type() == typeid(bool) &&
              copyArgs[1].type() == typeid(vrml_proc::math::TransformationMatrix) &&
              refArgs[0].get().type() == typeid(std::reference_wrapper<const VrmlNode>) &&
              refArgs[1].get().type() == typeid(std::reference_wrapper<const VrmlNode>) &&
              refArgs[2].get().type() == typeid(std::reference_wrapper<const VrmlNode>) &&
              refArgs[3].get().type() == typeid(std::reference_wrapper<const VrmlNode>) &&
              refArgs[4].get().type() == typeid(std::reference_wrapper<const bool>) &&
              refArgs[5].get().type() == typeid(std::reference_wrapper<const Int32Array>) &&
              refArgs[6].get().type() == typeid(std::reference_wrapper<const bool>) &&
              refArgs[7].get().type() == typeid(std::reference_wrapper<const bool>) &&
              refArgs[8].get().type() == typeid(std::reference_wrapper<const Int32Array>) &&
              refArgs[9].get().type() == typeid(std::reference_wrapper<const float32_t>) &&
              refArgs[10].get().type() == typeid(std::reference_wrapper<const Int32Array>) &&
              refArgs[11].get().type() == typeid(std::reference_wrapper<const bool>) &&
              refArgs[12].get().type() == typeid(std::reference_wrapper<const bool>) &&
              refArgs[13].get().type() == typeid(std::reference_wrapper<const Int32Array>)) {
            to_geom::action::IndexedFaceSetAction::Properties properties{
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

            return std::make_shared<to_geom::action::IndexedFaceSetAction>(
                properties, to_geom::action::GeometryAction::Properties{
                                std::any_cast<bool>(copyArgs[0]),
                                std::any_cast<vrml_proc::math::TransformationMatrix>(copyArgs[1])});
          }

          vrml_proc::core::logger::LogFatal("Invalid arguments for IndexedFaceSetAction!", LOGGING_INFO);
          throw std::runtime_error("Invalid arguments for IndexedFaceSetAction");
        });

    actionMap.AddAction(
        "IndexedLineSet", [](const vrml_proc::action::ConversionContextActionMap<
                                 to_geom::conversion_context::MeshTaskConversionContext>::ReferencedArguments& refArgs,
                             const vrml_proc::action::ConversionContextActionMap<
                                 to_geom::conversion_context::MeshTaskConversionContext>::CopiedArguments& copyArgs) {
          using vrml_proc::parser::VrmlNode;
          using vrml_proc::parser::Int32Array;
          using vrml_proc::parser::float32_t;

          if (refArgs.size() == 5 && copyArgs.size() == 2 && copyArgs[0].type() == typeid(bool) &&
              copyArgs[1].type() == typeid(vrml_proc::math::TransformationMatrix) &&
              refArgs[0].get().type() == typeid(std::reference_wrapper<const VrmlNode>) &&
              refArgs[1].get().type() == typeid(std::reference_wrapper<const VrmlNode>) &&
              refArgs[2].get().type() == typeid(std::reference_wrapper<const Int32Array>) &&
              refArgs[3].get().type() == typeid(std::reference_wrapper<const bool>) &&
              refArgs[4].get().type() == typeid(std::reference_wrapper<const Int32Array>)) {
            to_geom::action::IndexedLineSetAction::Properties properties{
                std::any_cast<std::reference_wrapper<const VrmlNode>>(refArgs[0]),
                std::any_cast<std::reference_wrapper<const VrmlNode>>(refArgs[1]),
                std::any_cast<std::reference_wrapper<const Int32Array>>(refArgs[2]),
                std::any_cast<std::reference_wrapper<const bool>>(refArgs[3]),
                std::any_cast<std::reference_wrapper<const Int32Array>>(refArgs[4])};

            return std::make_shared<to_geom::action::IndexedLineSetAction>(
                properties, to_geom::action::GeometryAction::Properties{
                                std::any_cast<bool>(copyArgs[0]),
                                std::any_cast<vrml_proc::math::TransformationMatrix>(copyArgs[1])});
          }

          vrml_proc::core::logger::LogFatal("Invalid arguments for IndexedLineSetAction!", LOGGING_INFO);
          throw std::runtime_error("IndexedLineSetAction");
        });

    return actionMap;
  }
}  // namespace to_geom::conversion_context
