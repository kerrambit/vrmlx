#pragma once

#include <memory>

#include <result.hpp>

#include "AppearanceHandler.hpp"
#include "BoxHandler.hpp"
#include "ColorHandler.hpp"
#include "ConversionContextActionMap.hpp"
#include "CoordinateHandler.hpp"
#include "Error.hpp"
#include "FormatString.hpp"
#include "GroupHandler.hpp"
#include "Hash.hpp"
#include "ImageTextureHandler.hpp"
#include "IndexedFaceSetHandler.hpp"
#include "IndexedLineSetHandler.hpp"
#include "Logger.hpp"
#include "MaterialHandler.hpp"
#include "NodeDescriptor.hpp"
#include "NodeDescriptorMap.hpp"
#include "NodeTraversorError.hpp"
#include "NodeView.hpp"
#include "NormalHandler.hpp"
#include "PixelTextureHandler.hpp"
#include "ShapeHandler.hpp"
#include "SwitchHandler.hpp"
#include "TextureCoordinateHandler.hpp"
#include "TextureTransformHandler.hpp"
#include "TransformHandler.hpp"
#include "TraversorResult.hpp"
#include "VrmlCanonicalHeaders.hpp"
#include "VrmlNode.hpp"
#include "VrmlNodeTraversorParameters.hpp"
#include "WorldInfoHandler.hpp"
#include "ConversionContextable.hpp"
#include "ConeHandler.hpp"
#include "CylinderHandler.hpp"
#include "ElevationGridHandler.hpp"
#include "ExtrusionHandler.hpp"
#include "PointSetHandler.hpp"
#include "SphereHandler.hpp"

// ------------------------------------------------------------------------- //

/**
 * @brief Helper function which tried to find and execute action.
 *
 * @param header node name
 * @param params parameters from traversor
 * @param actionMap action map from traversor
 * @param nd node view of the node
 *
 * @returns pointer to conversion context, otherwise an error object if any problem occurs
 */
template <ConversionContextable ConversionContext>
static vrml_proc::traversor::TraversorResult<ConversionContext> FindAndRunHandler(const std::string& header,
    const vrml_proc::traversor::VrmlNodeTraversorParameters& params,
    const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap,
    std::shared_ptr<vrml_proc::traversor::node_descriptor::NodeView> nd) {  //

  using namespace vrml_proc::core::utils;
  using namespace vrml_proc::core::error;
  using namespace vrml_proc::traversor::handler;
  using namespace vrml_proc::traversor::error;
  using namespace vrml_proc::traversor::node_descriptor;

  // Handler is found using precomputed hashes of each canonical node name.
  vrml_proc::traversor::TraversorResult<ConversionContext> handlerResult;
  switch (Hash(ConvertToCanonicalHeader(header))) {
    case CanonicalHeaderHashes::WorldInfo:
      handlerResult = WorldInfoHandler::Handle(params, actionMap, nd);
      break;
    case CanonicalHeaderHashes::Group:
      handlerResult = GroupHandler::Handle(params, actionMap, nd);
      break;
    case CanonicalHeaderHashes::Transform:
      handlerResult = TransformHandler::Handle(params, actionMap, nd);
      break;
    case CanonicalHeaderHashes::Shape:
      handlerResult = ShapeHandler::Handle(params, actionMap, nd);
      break;
    case CanonicalHeaderHashes::IndexedFaceSet:
      handlerResult = IndexedFaceSetHandler::Handle(params, actionMap, nd);
      break;
    case CanonicalHeaderHashes::IndexedLineSet:
      handlerResult = IndexedLineSetHandler::Handle(params, actionMap, nd);
      break;
    case CanonicalHeaderHashes::Coordinate:
      handlerResult = CoordinateHandler::Handle(params, actionMap, nd);
      break;
    case CanonicalHeaderHashes::Normal:
      handlerResult = NormalHandler::Handle(params, actionMap, nd);
      break;
    case CanonicalHeaderHashes::TextureCoordinate:
      handlerResult = TextureCoordinateHandler::Handle(params, actionMap, nd);
      break;
    case CanonicalHeaderHashes::Color:
      handlerResult = ColorHandler::Handle(params, actionMap, nd);
      break;
    case CanonicalHeaderHashes::Box:
      handlerResult = BoxHandler::Handle(params, actionMap, nd);
      break;
    case CanonicalHeaderHashes::Switch:
      handlerResult = SwitchHandler::Handle(params, actionMap, nd);
      break;
    case CanonicalHeaderHashes::Material:
      handlerResult = MaterialHandler::Handle(params, actionMap, nd);
      break;
    case CanonicalHeaderHashes::ImageTexture:
      handlerResult = ImageTextureHandler::Handle(params, actionMap, nd);
      break;
    case CanonicalHeaderHashes::PixelTexture:
      handlerResult = PixelTextureHandler::Handle(params, actionMap, nd);
      break;
    case CanonicalHeaderHashes::TextureTransform:
      handlerResult = TextureTransformHandler::Handle(params, actionMap, nd);
      break;
    case CanonicalHeaderHashes::Appearance:
      handlerResult = AppearanceHandler::Handle(params, actionMap, nd);
      break;
    case CanonicalHeaderHashes::Cone:
      handlerResult = ConeHandler::Handle(params, actionMap, nd);
      break;
    case CanonicalHeaderHashes::Cylinder:
      handlerResult = CylinderHandler::Handle(params, actionMap, nd);
      break;
    case CanonicalHeaderHashes::ElevationGrid:
      handlerResult = ElevationGridHandler::Handle(params, actionMap, nd);
      break;
    case CanonicalHeaderHashes::Extrusion:
      handlerResult = ExtrusionHandler::Handle(params, actionMap, nd);
      break;
    case CanonicalHeaderHashes::PointSet:
      handlerResult = PointSetHandler::Handle(params, actionMap, nd);
      break;
    case CanonicalHeaderHashes::Sphere:
      handlerResult = SphereHandler::Handle(params, actionMap, nd);
      break;
    default:
      break;
  }

  return handlerResult;
}

namespace vrml_proc::traversor::VrmlNodeTraversor {
  /**
   * @brief Traverses the VRML node. When given node is visited, an appropriate action is found and executed.
   * Accumulates the ConversionContext result
   *
   * @tparam ConversionContext type of conversion context (result type of the traversal)
   * @param params parameters for the traversal
   * @param actionMap map with defined actions
   */
  template <ConversionContextable ConversionContext>
  inline TraversorResult<ConversionContext> Traverse(VrmlNodeTraversorParameters params,
      const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap) {  //

    using namespace vrml_proc::core::logger;
    using namespace vrml_proc::core::utils;
    using namespace vrml_proc::traversor::handler;
    using namespace vrml_proc::traversor::error;
    using namespace vrml_proc::traversor::node_descriptor;

    bool ignoreUnknownNodeFlag = params.config.ignoreUnknownNode;

    LogInfo(FormatString("Find handler for VRML node with name <", params.node.header, ">."), LOGGING_INFO);

    if (params.node.header.empty()) {
      LogInfo("Handle empty VRML node.", LOGGING_INFO);
      return std::make_shared<ConversionContext>();
    }

    // Find appropriate node descriptor. If it is not found, the current node is not a valid node.
    auto ndResult = CreateNodeDescriptor(params.node.header);
    if (!ndResult.has_value()) {
      if (ignoreUnknownNodeFlag) {
        LogInfo(FormatString("No handler for VRML node with name <", params.node.header,
                    "> was found. The unknown node will be ignored."),
            LOGGING_INFO);
        return std::make_shared<ConversionContext>();
      }
      LogError(FormatString(
                   "No handler for VRML node with name <", params.node.header, "> was found! It is unknown VRML node!"),
          LOGGING_INFO);
      std::shared_ptr<UnknownVrmlNode> innerError = std::make_shared<UnknownVrmlNode>(params.node.header);
      return cpp::fail(std::make_shared<NodeTraversorError>(innerError, params.node));
    }

    // Validate node using node descriptor.
    auto& nd = ndResult.value();
    auto validationResult = nd.Validate(params.node, params.manager);
    if (validationResult.has_error()) {
      LogError(FormatString("Validation for node <", params.node.header, "> failed!"), LOGGING_INFO);
      return cpp::fail(std::make_shared<NodeTraversorError>(validationResult.error(), params.node));
    }

    // Find and run handler for node.
    auto handlerResult =
        FindAndRunHandler<ConversionContext>(params.node.header, params, actionMap, validationResult.value());
    if (handlerResult.has_error()) {
      return cpp::fail(std::make_shared<NodeTraversorError>(handlerResult.error(), params.node));
    }

    return handlerResult;
  }
}  // namespace vrml_proc::traversor::VrmlNodeTraversor
