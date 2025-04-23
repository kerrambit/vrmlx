#pragma once

#include <memory>

#include <NodeView.hpp>
#include "ConversionContextable.hpp"
#include "VrmlProcConfig.hpp"

namespace vrml_proc::traversor::handler {
  /**
   * @brief Represents an object which is passed from handler to a given action via ConversionContextActionMap.
   *
   * It groups a representation of VRML node (NodeView) and results of traversal as ccGroup (for a vector of
   * ConversionContexts) or cc_i for individual ConversionContexts.
   */
  template <ConversionContextable ConversionContext>
  struct HandlerToActionBundle {
    /**
     * @brief Constructs a default object.
     */
    HandlerToActionBundle()
        : nodeView(nullptr), ccGroup({}), cc1(nullptr), cc2(nullptr), cc3(nullptr), config(nullptr) {}

    /**
     * @brief Constructs an object.
     *
     * @param nodeView node view
     */
    HandlerToActionBundle(std::shared_ptr<vrml_proc::traversor::node_descriptor::NodeView> nodeView)
        : nodeView(nodeView), ccGroup({}), cc1(nullptr), cc2(nullptr), cc3(nullptr), config(nullptr) {}

    /**
     * @brief Constructs an object.
     *
     * @param nodeView node view
     * @param config configuration file
     */
    HandlerToActionBundle(std::shared_ptr<vrml_proc::traversor::node_descriptor::NodeView> nodeView,
        std::shared_ptr<vrml_proc::core::config::VrmlProcConfig> config)
        : nodeView(nodeView), ccGroup({}), cc1(nullptr), cc2(nullptr), cc3(nullptr), config(nullptr), config(config) {}

    /**
     * @brief Constructs an object.
     *
     * @param nodeView node view
     * @param ccGroup vector of ConversionContexts
     */
    HandlerToActionBundle(std::shared_ptr<vrml_proc::traversor::node_descriptor::NodeView> nodeView,
        const std::vector<std::shared_ptr<ConversionContext>>& ccGroup)
        : nodeView(nodeView, ccGroup), ccGroup(ccGroup), cc1(nullptr), cc2(nullptr), cc3(nullptr), config(nullptr) {}

    std::shared_ptr<vrml_proc::traversor::node_descriptor::NodeView> nodeView;
    std::vector<std::shared_ptr<ConversionContext>> ccGroup;
    std::shared_ptr<ConversionContext> cc1;
    std::shared_ptr<ConversionContext> cc2;
    std::shared_ptr<ConversionContext> cc3;
    std::shared_ptr<vrml_proc::core::config::VrmlProcConfig> config;
  };
}  // namespace vrml_proc::traversor::handler
