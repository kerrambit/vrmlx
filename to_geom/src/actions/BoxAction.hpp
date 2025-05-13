#pragma once

#include <memory>

#include "GeometryAction.hpp"
#include "MeshTaskConversionContext.hpp"
#include "Vec3f.hpp"

#include "ToGeomExport.hpp"

namespace to_geom::action {
  /**
   * @brief Represents an action for the Box VRML node.
   *
   * Converts a VRML `Box` node into a geometric representation.
   * Inherits from `GeometryAction` to integrate with the conversion system.
   */
  class TOGEOM_API BoxAction : public to_geom::action::GeometryAction {
   public:
    /**
     * @brief Properties for `BoxAction`. See VRML 2.0 specification for more information.
     */
    struct Properties {
      std::reference_wrapper<const vrml_proc::parser::model::Vec3f> size;
    };

    /**
     * @brief Constructs a BoxAction with specified properties.
     * @param properties properties for BoxAction
     * @param geometryProperties geometry properties
     */
    BoxAction(Properties properties, GeometryAction::Properties geometryProperties);

    /**
     * @brief Executes the conversion of the Box node to a mesh representation.
     * @return A shared pointer to the generated mesh conversion context.
     */
    std::shared_ptr<to_geom::conversion_context::MeshTaskConversionContext> Execute() override;

   private:
    Properties m_properties;
  };
}  // namespace to_geom::action
