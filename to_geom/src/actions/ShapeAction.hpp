#pragma once

#include <memory>
#include <vector>

#include "ConversionContextAction.hpp"
#include "MeshTaskConversionContext.hpp"

#include "VrmlProcessingExport.hpp"

namespace to_geom {
  namespace action {
    /**
     * @brief Represents an action for Shape VRML node.
     *
     * Process a VRML `Shape` node.
     * Inherits from `ConversionContextAction<MeshTaskConversionContext>` to integrate with the conversion system.
     */
    class VRMLPROCESSING_API ShapeAction
        : public vrml_proc::action::ConversionContextAction<to_geom::conversion_context::MeshTaskConversionContext> {
     public:
      /**
       * @brief Properties for `ShapeAction`. See VRML 2.0 specification for more information.
       */
      struct Properties {
        std::shared_ptr<to_geom::conversion_context::MeshTaskConversionContext> appearance;
        std::shared_ptr<to_geom::conversion_context::MeshTaskConversionContext> geometry;
      };

      /**
       * @brief Constructs a ShapeAction with specified properties.
       * @param properties properties for ShapeAction
       */
      ShapeAction(Properties properties);

      /**
       * @brief Overriden implemented interface method from `BaseConversionContextAction`. The method is focused only on
       * `m_geometry` member field.
       *
       * @returns shared pointer owning the object of merged ConversionContext objects
       */
      std::shared_ptr<to_geom::conversion_context::MeshTaskConversionContext> Execute() override;

     private:
      Properties m_properties;
    };
  }  // namespace action
}  // namespace to_geom
