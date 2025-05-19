#pragma once

#include <cassert>
#include <memory>

#include <result.hpp>

#include "AppearanceHandler.hpp"
#include "BasicHandler.hpp"
#include "CollisionHandler.hpp"
#include "ConversionContextable.hpp"
#include "ConversionContextActionMap.hpp"
#include "ElevationGridHandler.hpp"
#include "Error.hpp"
#include "FormatString.hpp"
#include "GroupingHandler.hpp"
#include "Hash.hpp"
#include "IndexedFaceSetHandler.hpp"
#include "IndexedLineSetHandler.hpp"
#include "LODHandler.hpp"
#include "Logger.hpp"
#include "NodeDescriptor.hpp"
#include "NodeDescriptorMap.hpp"
#include "NodeTraversorError.hpp"
#include "NodeView.hpp"
#include "ShapeHandler.hpp"
#include "SwitchHandler.hpp"
#include "TextHandler.hpp"
#include "TransformHandler.hpp"
#include "TraversorResult.hpp"
#include "VrmlCanonicalHeaderHashes.hpp"
#include "VrmlHeaders.hpp"
#include "VrmlNode.hpp"
#include "VrmlNodeManager.hpp"
#include "VrmlNodeTraversorParameters.hpp"
#include "VrmlProcConfig.hpp"

// ------------------------------------------------------------------------- //

namespace vrml_proc::traversor {
  /**
   * @brief Class responsible for VrmlNode traversal.
   * @tparam ConversionContext conversion context
   */
  template <vrml_proc::core::contract::ConversionContextable ConversionContext>
  class VrmlNodeTraversor {
   public:
    /**
     * @brief Constructs new object,
     *
     * @param manager manager
     * @param config configuration file
     * @param actionMap map with stored actions
     * @param headersMap synonyms-to-canonical names mappings
     */
    VrmlNodeTraversor(const vrml_proc::parser::service::VrmlNodeManager& manager,
        std::shared_ptr<vrml_proc::core::config::VrmlProcConfig> config,
        const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap,
        const vrml_proc::traversor::node_descriptor::VrmlHeaders& headersMap)
        : m_manager(manager), m_config(config), m_actionMap(actionMap), m_headersMap(headersMap) {}

    /**
     * @brief Traverses the VRML node. When given node is visited, an appropriate action is found and executed.
     * Accumulates the ConversionContext result
     *
     * @param params parameters for the traversal
     * @returns Conversion context or error
     */
    TraversorResult<ConversionContext> Traverse(VrmlNodeTraversorParameters params) {  //

      using namespace vrml_proc::core::logger;
      using namespace vrml_proc::core::utils;
      using namespace vrml_proc::traversor::handler;
      using namespace vrml_proc::traversor::error;
      using namespace vrml_proc::traversor::node_descriptor;

      // ---------------------------------------------------

      bool ignoreUnknownNodeFlag = m_config->ignoreUnknownNode;

      LogDebug(FormatString("Find handler for VRML node with name <", params.node.get().header, ">."), LOGGING_INFO);

      if (params.node.get().header.empty()) {
        LogDebug("Handle empty VRML node.", LOGGING_INFO);
        return std::make_shared<ConversionContext>();
      }

      // Find canonical name.
      auto canonicalHeader = m_headersMap.ConvertToCanonicalHeader(params.node.get().header);

      // Create appropriate node descriptor. If descriptor cannot be created, it means we have an unknown node name.
      auto nodeDescriptorResult = CreateNodeDescriptor(canonicalHeader);
      if (!nodeDescriptorResult.has_value()) {  //
        if (ignoreUnknownNodeFlag) {
          LogInfo(FormatString("No handler for VRML node with name <", canonicalHeader,
                      "> was found. The unknown node will be ignored."),
              LOGGING_INFO);
          return std::make_shared<ConversionContext>();
        }

        LogError(FormatString(
                     "No handler for VRML node with name <", canonicalHeader, "> was found! It is unknown VRML node!"),
            LOGGING_INFO);
        std::shared_ptr<UnknownVrmlNode> innerError = std::make_shared<UnknownVrmlNode>(canonicalHeader);
        return cpp::fail(std::make_shared<NodeTraversorError>(innerError, params.node.get()));
      }

      // Validate node using node descriptor.
      auto& nodeDescriptor = nodeDescriptorResult.value();
      auto validationResult = nodeDescriptor.Validate(params.node.get(), m_manager, m_headersMap, false);
      if (validationResult.has_error()) {
        LogError(FormatString("Validation for node <", canonicalHeader, "> failed!"), LOGGING_INFO);
        return cpp::fail(std::make_shared<NodeTraversorError>(validationResult.error(), params.node.get()));
      }

      // Find and run handler for node.
      auto handlerResult = FindAndRunHandler(canonicalHeader, params, validationResult.value());
      if (handlerResult.has_error()) {
        return cpp::fail(std::make_shared<NodeTraversorError>(handlerResult.error(), params.node.get()));
      }

      return handlerResult;
    }

   private:
    const vrml_proc::parser::service::VrmlNodeManager& m_manager;
    std::shared_ptr<vrml_proc::core::config::VrmlProcConfig> m_config;
    const vrml_proc::action::ConversionContextActionMap<ConversionContext>& m_actionMap;
    const vrml_proc::traversor::node_descriptor::VrmlHeaders& m_headersMap;

    /**
     * @brief Helper function which tries to find and execute action.
     *
     * @param header canonical name of node
     * @param params parameters from traversor
     * @param nodeView node view of the node
     *
     * @returns pointer to conversion context, otherwise an error object if any problem occurs
     */
    TraversorResult<ConversionContext> FindAndRunHandler(const std::string& header,
        const VrmlNodeTraversorParameters& params,
        std::shared_ptr<vrml_proc::traversor::node_descriptor::NodeView> nodeView) {  //

      using namespace vrml_proc::core::utils;
      using namespace vrml_proc::core::error;
      using namespace vrml_proc::traversor::handler;
      using namespace vrml_proc::traversor::error;
      using namespace vrml_proc::traversor::node_descriptor;

      // ---------------------------------------------------

      TraversorResult<ConversionContext> handlerResult = std::make_shared<ConversionContext>();
      HandlerParameters<ConversionContext> inputHandlerParameters(
          nodeView, params.IsDescendantOfShape, params.transformation, m_manager, m_actionMap, m_config, m_headersMap);

      switch (Hash(header)) {
        case CanonicalHeaderHashes::Group:
          handlerResult = GroupingHandler::Handle(inputHandlerParameters);
          break;
        case CanonicalHeaderHashes::Shape:
          handlerResult = ShapeHandler::Handle(inputHandlerParameters);
          break;
        case CanonicalHeaderHashes::Appearance:
          handlerResult = AppearanceHandler::Handle(inputHandlerParameters);
          break;
        case CanonicalHeaderHashes::Coordinate:
          handlerResult = BasicHandler::Handle(inputHandlerParameters);
          break;
        case CanonicalHeaderHashes::Normal:
          handlerResult = BasicHandler::Handle(inputHandlerParameters);
          break;
        case CanonicalHeaderHashes::Transform:
          handlerResult = TransformHandler::Handle(inputHandlerParameters);
          break;
        case CanonicalHeaderHashes::Switch:
          handlerResult = SwitchHandler::Handle(inputHandlerParameters);
          break;
        case CanonicalHeaderHashes::Material:
          handlerResult = BasicHandler::Handle(inputHandlerParameters);
          break;
        case CanonicalHeaderHashes::IndexedFaceSet:
          handlerResult = IndexedFaceSetHandler::Handle(inputHandlerParameters);
          break;
        case CanonicalHeaderHashes::IndexedLineSet:
          handlerResult = IndexedLineSetHandler::Handle(inputHandlerParameters);
          break;
        case CanonicalHeaderHashes::TextureCoordinate:
          handlerResult = BasicHandler::Handle(inputHandlerParameters);
          break;
        case CanonicalHeaderHashes::Box:
          handlerResult = BasicHandler::Handle(inputHandlerParameters);
          break;
        case CanonicalHeaderHashes::ImageTexture:
          handlerResult = BasicHandler::Handle(inputHandlerParameters);
          break;
        case CanonicalHeaderHashes::PixelTexture:
          handlerResult = BasicHandler::Handle(inputHandlerParameters);
          break;
        case CanonicalHeaderHashes::TextureTransform:
          handlerResult = BasicHandler::Handle(inputHandlerParameters);
          break;
        case CanonicalHeaderHashes::Cone:
          handlerResult = BasicHandler::Handle(inputHandlerParameters);
          break;
        case CanonicalHeaderHashes::Cylinder:
          handlerResult = BasicHandler::Handle(inputHandlerParameters);
          break;
        case CanonicalHeaderHashes::ElevationGrid:
          handlerResult = ElevationGridHandler::Handle(inputHandlerParameters);
          break;
        case CanonicalHeaderHashes::Extrusion:
          handlerResult = BasicHandler::Handle(inputHandlerParameters);
          break;
        case CanonicalHeaderHashes::PointSet:
          handlerResult = BasicHandler::Handle(inputHandlerParameters);
          break;
        case CanonicalHeaderHashes::Sphere:
          handlerResult = BasicHandler::Handle(inputHandlerParameters);
          break;
        case CanonicalHeaderHashes::WorldInfo:
          handlerResult = BasicHandler::Handle(inputHandlerParameters);
          break;
        case CanonicalHeaderHashes::Anchor:
          handlerResult = GroupingHandler::Handle(inputHandlerParameters);
          break;
        case CanonicalHeaderHashes::Billboard:
          handlerResult = GroupingHandler::Handle(inputHandlerParameters);
          break;
        case CanonicalHeaderHashes::Collision:
          handlerResult = CollisionHandler::Handle(inputHandlerParameters);
          break;
        case CanonicalHeaderHashes::Inline:
          handlerResult = BasicHandler::Handle(inputHandlerParameters);
          break;
        case CanonicalHeaderHashes::LOD:
          handlerResult = LODHandler::Handle(inputHandlerParameters);
          break;
        case CanonicalHeaderHashes::FontStyle:
          handlerResult = BasicHandler::Handle(inputHandlerParameters);
          break;
        case CanonicalHeaderHashes::Text:
          handlerResult = TextHandler::Handle(inputHandlerParameters);
          break;
        default:
          assert(false && "Cannot find handler for VRML node. Unknown VRML node should not get into this stage!");
          break;
      }

      return handlerResult;
    }
  };
}  // namespace vrml_proc::traversor
