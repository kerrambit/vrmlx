#pragma once

#include <memory>

#include "ConversionContextable.hpp"
#include "ConversionContextActionMap.hpp"
#include "NodeView.hpp"
#include "TransformationMatrix.hpp"
#include "VrmlHeaders.hpp"
#include "VrmlNodeManager.hpp"
#include "VrmlProcConfig.hpp"

namespace vrml_proc::traversor::handler {
  /**
   * @brief Represents a structure for all parameters passed to a handler.
   * @tparam ConversionContext conversion context
   */
  template <vrml_proc::core::contract::ConversionContextable ConversionContext>
  struct HandlerParameters {
    /**
     * @brief Constructs a new HandlerParameters object.
     *
     * @param nodeView shared pointer to the node view being processed
     * @param isDescendantOfShape flag indicating whether the node is a descendant of a Shape node
     * @param transformation transformation matrix accumulated so far
     * @param manager reference to the VRML node manager
     * @param actionMap map linking node headers to conversion conetxt actions
     * @param config shared pointer to the configuration object
     * @param headersMap map for canonicalizing node headers
     */
    HandlerParameters(std::shared_ptr<vrml_proc::traversor::node_descriptor::NodeView> nodeView,
        bool isDescendantOfShape,
        const vrml_proc::math::TransformationMatrix& transformation,
        const vrml_proc::parser::service::VrmlNodeManager& manager,
        const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap,
        std::shared_ptr<vrml_proc::core::config::VrmlProcConfig> config,
        const vrml_proc::traversor::node_descriptor::VrmlHeaders& headersMap)
        : nodeView(nodeView),
          IsDescendantOfShape(isDescendantOfShape),
          transformation(transformation),
          manager(manager),
          actionMap(actionMap),
          config(config),
          headersMap(headersMap) {}

    std::shared_ptr<vrml_proc::traversor::node_descriptor::NodeView> nodeView;
    bool IsDescendantOfShape;
    vrml_proc::math::TransformationMatrix transformation;
    const vrml_proc::parser::service::VrmlNodeManager& manager;
    const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap;
    std::shared_ptr<vrml_proc::core::config::VrmlProcConfig> config;
    const vrml_proc::traversor::node_descriptor::VrmlHeaders& headersMap;
  };
}  // namespace vrml_proc::traversor::handler
