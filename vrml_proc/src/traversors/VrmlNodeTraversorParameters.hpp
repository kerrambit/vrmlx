#pragma once

#include "TransformationMatrix.hpp"
#include "VrmlNode.hpp"
#include "VrmlNodeManager.hpp"
#include "VrmlProcConfig.hpp"

namespace vrml_proc::traversor {
  /**
   * @brief Represents parameters passed to the node traversor.
   */
  struct VrmlNodeTraversorParameters {
    /**
     * @brief Constructs an object.
     *
     * @param node node to traverse
     * @param manager manager to use when resolving USE nodes
     * @param isDescendantOfShape flag if the node is descedent of a shape node
     * @param transformation current transformation in the form of a matrix
     * @param config configuration file
     */
    VrmlNodeTraversorParameters(const vrml_proc::parser::VrmlNode& node,
        const vrml_proc::parser::VrmlNodeManager& manager,
        bool isDescendantOfShape,
        vrml_proc::math::TransformationMatrix transformation,
        const vrml_proc::core::config::VrmlProcConfig& config)
        : node(node),
          manager(manager),
          IsDescendantOfShape(isDescendantOfShape),
          transformation(transformation),
          config(config) {}

    const vrml_proc::parser::VrmlNode& node;
    const vrml_proc::parser::VrmlNodeManager& manager;
    bool IsDescendantOfShape;
    vrml_proc::math::TransformationMatrix transformation;
    const vrml_proc::core::config::VrmlProcConfig& config;
  };
}  // namespace vrml_proc::traversor
