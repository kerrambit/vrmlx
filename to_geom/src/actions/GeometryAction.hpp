#pragma once

#include <memory>

#include "ConversionContextAction.hpp"
#include "MeshTaskConversionContext.hpp"
#include "TransformationMatrix.hpp"

namespace to_geom {
  namespace action {
    /**
     * @brief Represents a generic geometry node action. Geometry nodes are affected by their node ancestors such as
     * Transform or Shape.
     *
     * Inherits from `ConversionContextAction<MeshTaskConversionContext>` to integrate with the conversion system.
     */
    class GeometryAction
        : public vrml_proc::action::ConversionContextAction<to_geom::conversion_context::MeshTaskConversionContext> {
     public:
      /**
       * @brief Properties for `GeometryAction`.
       */
      struct Properties {
        /**
         * @brief Flag indicating if the node has a Shape as an ancestor.
         */
        bool containedByShape;
        /**
         * @brief Transformation matrix that should be applied to geometry nodes.
         */
        vrml_proc::math::TransformationMatrix matrix;
      };
      /**
       * @brief Constructs a GeometryAction with specified properties.
       * @param properties properties for GeometryAction
       */
      explicit GeometryAction(Properties properties) : m_geometryProperties(properties) {}

      /**
       * @brief Executes the conversion process for this action.
       *
       * This pure virtual function must be implemented by derived classes to
       * perform the conversion of a VRML node into a geometric representation.
       *
       * @return A shared pointer to the generated mesh conversion context.
       */
      virtual std::shared_ptr<to_geom::conversion_context::MeshTaskConversionContext> Execute() = 0;

     protected:
      Properties m_geometryProperties;
    };
  }  // namespace action
}  // namespace to_geom
