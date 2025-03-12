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
#include "Logger.hpp"
#include "MaterialHandler.hpp"
#include "NodeDescriptor.hpp"
#include "NodeTraversorError.hpp"
#include "NormalHandler.hpp"
#include "PixelTextureHandler.hpp"
#include "ShapeHandler.hpp"
#include "SwitchHandler.hpp"
#include "TextureCoordinateHandler.hpp"
#include "IndexedLineSetHandler.hpp"
#include "TextureTransformHandler.hpp"
#include "TransformHandler.hpp"
#include "VrmlNode.hpp"
#include "VrmlNodeTraversorParameters.hpp"
#include "WorldInfoHandler.hpp"
#include <NodeDescriptorMap.hpp>
#include <VrmlCanonicalHeaders.hpp>

#include "VrmlProcessingExport.hpp"

namespace vrml_proc::traversor::VrmlNodeTraversor {

  template <typename ConversionContext>
  VRMLPROCESSING_API inline cpp::result<std::shared_ptr<ConversionContext>,
                                        std::shared_ptr<vrml_proc::core::error::Error>>
  Traverse(vrml_proc::traversor::VrmlNodeTraversorParameters context,
           const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap) {
    using namespace vrml_proc::core::logger;
    using namespace vrml_proc::core::utils;
    using namespace vrml_proc::traversor::handler;
    using namespace vrml_proc::traversor::error;
    using namespace vrml_proc::traversor::node_descriptor;

    bool ignoreUnknownNodeFlag = context.config.ignoreUnknownNode;

    LogInfo(FormatString("Find handler for VRML node with name <", context.node.header, ">."), LOGGING_INFO);

    if (context.node.header.empty()) {
      LogInfo("Handle empty VRML node.", LOGGING_INFO);
      return std::make_shared<ConversionContext>();
    }

    std::string canonicalHeader;
    {
      auto it = GetHeaderToCanonicalNameMap().find(context.node.header);
      if (it != GetHeaderToCanonicalNameMap().end()) {
        canonicalHeader = it->second;
      }
    }

    auto descriptorMap = CreateNodeDescriptorMap();
    auto it = descriptorMap.find(canonicalHeader);
    NodeDescriptor nd;
    if (it != descriptorMap.end()) {
      nd = it->second();
      auto validationResult = nd.Validate(context.node, context.manager);
      if (validationResult.has_error()) {
        LogError(FormatString("Validation for node <", context.node.header, "> failed!"), LOGGING_INFO);
        return cpp::fail(std::make_shared<NodeTraversorError>(validationResult.error(), context.node));
      }
    } else {
      if (ignoreUnknownNodeFlag) {
        LogInfo(FormatString("No handler for VRML node with name <", context.node.header,
                             "> was found! The unknown node will be ignored."),
                LOGGING_INFO);
        return std::make_shared<ConversionContext>();
      }

      LogError(FormatString("No handler for VRML node with name <", context.node.header,
                            "> was found! It is unknown VRML node."),
               LOGGING_INFO);
      std::shared_ptr<UnknownVrmlNode> innerError = std::make_shared<UnknownVrmlNode>(context.node.header);
      return cpp::fail(std::make_shared<NodeTraversorError>(innerError, context.node));
    }

    cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> handlerResult;
    switch (Hash(canonicalHeader)) {
      case CanonicalHeaderHashes::WorldInfo:
        handlerResult = WorldInfoHandler::Handle(context, actionMap, nd);
        break;
      case CanonicalHeaderHashes::Group:
        handlerResult = GroupHandler::Handle(context, actionMap, nd);
        break;
      case CanonicalHeaderHashes::Transform:
        handlerResult = TransformHandler::Handle(context, actionMap, nd);
        break;
      case CanonicalHeaderHashes::Shape:
        handlerResult = ShapeHandler::Handle(context, actionMap, nd);
        break;
      case CanonicalHeaderHashes::IndexedFaceSet:
        handlerResult = IndexedFaceSetHandler::Handle(context, actionMap, nd);
        break;
      case CanonicalHeaderHashes::IndexedLineSet:
        handlerResult = IndexedLineSetHandler::Handle(context, actionMap, nd);
        break;
      case CanonicalHeaderHashes::Coordinate:
        handlerResult = CoordinateHandler::Handle(context, actionMap, nd);
        break;
      case CanonicalHeaderHashes::Normal:
        handlerResult = NormalHandler::Handle(context, actionMap, nd);
        break;
      case CanonicalHeaderHashes::TextureCoordinate:
        handlerResult = TextureCoordinateHandler::Handle(context, actionMap, nd);
        break;
      case CanonicalHeaderHashes::Color:
        handlerResult = ColorHandler::Handle(context, actionMap, nd);
        break;
      case CanonicalHeaderHashes::Box:
        handlerResult = BoxHandler::Handle(context, actionMap, nd);
        break;
      case CanonicalHeaderHashes::Switch:
        handlerResult = SwitchHandler::Handle(context, actionMap, nd);
        break;
      case CanonicalHeaderHashes::Material:
        handlerResult = MaterialHandler::Handle(context, actionMap, nd);
        break;
      case CanonicalHeaderHashes::ImageTexture:
        handlerResult = ImageTextureHandler::Handle(context, actionMap, nd);
        break;
      case CanonicalHeaderHashes::PixelTexture:
        handlerResult = PixelTextureHandler::Handle(context, actionMap, nd);
        break;
      case CanonicalHeaderHashes::TextureTransform:
        handlerResult = TextureTransformHandler::Handle(context, actionMap, nd);
        break;
      case CanonicalHeaderHashes::Appearance:
        handlerResult = AppearanceHandler::Handle(context, actionMap, nd);
        break;
      default:
        break;
    }

    if (handlerResult.has_error()) {
      return cpp::fail(std::make_shared<NodeTraversorError>(handlerResult.error(), context.node));
    }

    return handlerResult;
  }
}  // namespace vrml_proc::traversor::VrmlNodeTraversor