#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <vector>

#include "ConversionContextAction.hpp"
#include "MeshTaskConversionContext.hpp"

#include "VrmlProcessingExport.hpp"

namespace to_geom {
  namespace action {
    /**
     * @brief Represents an action for Switch VRML node.
     *
     * Process a VRML `Switch` node.
     * Inherits from `ConversionContextAction<MeshTaskConversionContext>` to integrate with the conversion system.
     */
    class VRMLPROCESSING_API SwitchAction
        : public vrml_proc::action::ConversionContextAction<to_geom::conversion_context::MeshTaskConversionContext> {
     public:
      /**
       * @brief Properties for `SwitchAction`. See VRML 2.0 specification for more information.
       */
      struct SwitchProperties {
        std::shared_ptr<to_geom::conversion_context::MeshTaskConversionContext> choice;
      };

      /**
       * @brief Constructs a SwitchAction with specified properties.
       * @param properties properties for SwitchAction
       */
      SwitchAction(SwitchProperties properties);

      /**
       * @brief Overriden implemented interface method from `BaseConversionContextAction`. The method simply merges
       * choice into new conversion context and returns it.
       *
       * @returns shared pointer owning the object of merged ConversionContext objects
       */
      std::shared_ptr<to_geom::conversion_context::MeshTaskConversionContext> Execute() override;

     private:
      SwitchProperties m_properties;
    };
  }  // namespace action
}  // namespace to_geom
