#pragma once

#include <functional>
#include <memory>

#include "GeometryAction.hpp"
#include "Int32Array.hpp"
#include "MeshTaskConversionContext.hpp"
#include "VrmlNode.hpp"
#include "VrmlUnits.hpp"

#include "ToGeomExport.hpp"

namespace to_geom::action {
  /**
   * @brief Represents an action for IndexedLineSet VRML node.
   *
   * Converts a VRML `IndexedLineSet` node into a geometric representation.
   * Inherits from `GeometryAction` to integrate with the conversion system.
   */
  class TOGEOM_API IndexedLineSetAction : public to_geom::action::GeometryAction {
   public:
    /**
     * @brief Properties for `IndexedLineSetAction`. See VRML 2.0 specification for more information.
     */
    struct Properties {
      std::reference_wrapper<const vrml_proc::parser::VrmlNode> coord;
      std::reference_wrapper<const vrml_proc::parser::Int32Array> coordIndex;
    };

    /**
     * @brief Constructs a IndexedLineSetAction with specified properties.
     * @param properties properties for IndexedLineSetAction
     * @param geometryProperties geometry properties
     */
    IndexedLineSetAction(Properties properties, GeometryAction::Properties geometryProperties);

    /**
     * @brief Executes the conversion of the IndexedLineSet node to a mesh representation.
     * @return A shared pointer to the generated mesh conversion context.
     */
    std::shared_ptr<to_geom::conversion_context::MeshTaskConversionContext> Execute() override;

   private:
    Properties m_properties;
  };
}  // namespace to_geom::action
