#pragma once

#include <memory>

#include <NodeView.hpp>

namespace vrml_proc::traversor::handler {
  /**
   * @brief Represents an object which is passed from handler to a given action via ConversionContextActionMap.
   *
   * It groups a representation of VRML node (NodeView) and results of traversal as ccGroup (for a vector of
   * ConversionContexts) or cc_i for individual ConversionContexts.
   */
  template <typename ConversionContext>
  struct HandlerToActionBundle {
    /**
     * @brief Constructs a default object.
     */
    HandlerToActionBundle() : nodeView(nullptr), ccGroup({}), cc1(nullptr), cc2(nullptr), cc3(nullptr) {}

    /**
     * @brief Constructs an object.
     *
     * @param nodeView node view
     */
    HandlerToActionBundle(std::shared_ptr<vrml_proc::traversor::node_descriptor::NodeView> nodeView)
        : nodeView(nodeView), ccGroup({}), cc1(nullptr), cc2(nullptr), cc3(nullptr) {}

    /**
     * @brief Constructs an object.
     *
     * @param nodeView node view
     * @param ccGroup vector of ConversionContexts
     */
    HandlerToActionBundle(std::shared_ptr<vrml_proc::traversor::node_descriptor::NodeView> nodeView,
        const std::vector<std::shared_ptr<ConversionContext>>& ccGroup)
        : nodeView(nodeView, ccGroup), ccGroup(ccGroup), cc1(nullptr), cc2(nullptr), cc3(nullptr) {}

    std::shared_ptr<vrml_proc::traversor::node_descriptor::NodeView> nodeView;
    std::vector<std::shared_ptr<ConversionContext>> ccGroup;
    std::shared_ptr<ConversionContext> cc1;
    std::shared_ptr<ConversionContext> cc2;
    std::shared_ptr<ConversionContext> cc3;
  };
}  // namespace vrml_proc::traversor::handler
