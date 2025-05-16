#pragma once

#include <functional>

#include "TransformationMatrix.hpp"
#include "VrmlNode.hpp"

namespace vrml_proc::traversor {
  /**
   * @brief Represents parameters passed to the node traversor.
   */
  struct VrmlNodeTraversorParameters {
    /**
     * @brief Constructs new object.
     *
     * @param node node to traverse
     * @param isDescendantOfShape flag if the node is descedent of a shape node
     * @param transformation current transformation in the form of a matrix
     */
    VrmlNodeTraversorParameters(std::reference_wrapper<const vrml_proc::parser::model::VrmlNode> node,
        bool isDescendantOfShape,
        const vrml_proc::math::TransformationMatrix& transformation)
        : node(node), IsDescendantOfShape(isDescendantOfShape), transformation(transformation) {}

    std::reference_wrapper<const vrml_proc::parser::model::VrmlNode> node;
    bool IsDescendantOfShape;
    vrml_proc::math::TransformationMatrix transformation;
  };
}  // namespace vrml_proc::traversor
