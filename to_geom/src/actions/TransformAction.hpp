#pragma once

#include <memory>
#include <vector>

#include "ConversionContextAction.hpp"
#include "MeshTaskConversionContext.hpp"
#include "Vec3f.hpp"
#include "Vec4f.hpp"

#include "VrmlProcessingExport.hpp"

namespace to_geom {
  namespace action {
    /**
     * @brief Represents an action for the Transform VRML node.
     *
     * Inherits from `ConversionContextAction<MeshTaskConversionContext>` to integrate with the conversion system.
     */
    class VRMLPROCESSING_API TransformAction
        : public vrml_proc::action::ConversionContextAction<to_geom::conversion_context::MeshTaskConversionContext> {
     public:
      /**
       * @brief Properties for `TransformAction`. See VRML 2.0 specification for more information.
       */
      struct Properties {
        std::reference_wrapper<const vrml_proc::parser::Vec3f> center;
        std::reference_wrapper<const vrml_proc::parser::Vec4f> rotation;
        std::reference_wrapper<const vrml_proc::parser::Vec3f> scale;
        std::reference_wrapper<const vrml_proc::parser::Vec4f> scaleOrientation;
        std::reference_wrapper<const vrml_proc::parser::Vec3f> translation;
        std::reference_wrapper<const vrml_proc::parser::Vec3f> bboxCenter;
        std::reference_wrapper<const vrml_proc::parser::Vec3f> bboxSize;
        std::vector<std::shared_ptr<to_geom::conversion_context::MeshTaskConversionContext>> children;
      };

      /**
       * @brief Constructs a TransformAction with specified properties.
       * @param properties properties for TransformAction
       */
      TransformAction(Properties properties);

      /**
       * @brief The method is focused only on children field. All children are merged together and returned as a
       * pointer.
       * @return A shared pointer to the MeshTaskConversionContext.
       */
      std::shared_ptr<to_geom::conversion_context::MeshTaskConversionContext> Execute() override;

     private:
      Properties m_properties;
    };
  }  // namespace action
}  // namespace to_geom
