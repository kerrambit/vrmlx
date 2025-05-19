#pragma once

#include <functional>
#include <memory>

#include "GeometryAction.hpp"
#include "Int32Array.hpp"
#include "MeshTaskConversionContext.hpp"
#include "VrmlNode.hpp"
#include "VrmlUnits.hpp"
#include "ToGeomConfig.hpp"

#include "ToGeomExport.hpp"

namespace to_geom::action {
  /**
   * @brief Represents an action for IndexedFaceSet VRML node.
   *
   * Converts a VRML `IndexedFaceSet` node into a geometric representation.
   * Inherits from `GeometryAction`.
   */
  class TOGEOM_API IndexedFaceSetAction : public to_geom::action::GeometryAction {
   public:
    /**
     * @brief Properties for `IndexedFaceSetAction`. See VRML 2.0 specification for more information.
     */
    struct Properties {
      std::reference_wrapper<const vrml_proc::parser::model::VrmlNode> coord;
      std::reference_wrapper<const bool> convex;
      std::reference_wrapper<const vrml_proc::parser::model::Int32Array> coordIndex;
      std::shared_ptr<to_geom::core::config::ToGeomConfig> config;
    };

    /**
     * @brief Constructs a IndexedFaceSetAction with specified properties.
     * @param properties properties for IndexedFaceSetAction
     * @param geometryProperties geometry properties
     */
    IndexedFaceSetAction(Properties properties, GeometryAction::Properties geometryProperties);

    /**
     * @brief The method is focused only on geometry field, which is only passed through this method with no
     * modification.
     *
     * @return A shared pointer to the MeshTaskConversionContext.
     */
    std::shared_ptr<to_geom::conversion_context::MeshTaskConversionContext> Execute() override;

   private:
    Properties m_properties;
  };
}  // namespace to_geom::action
