#pragma once

#include <memory>
#include <vector>

#include "ConversionContextAction.hpp"
#include "MeshTaskConversionContext.hpp"
#include "Vec3f.hpp"

#include "ToGeomExport.hpp"

namespace to_geom::action {
  /**
   * @brief Represents an action for the Group VRML node.
   *
   * Inherits from `ConversionContextAction<MeshTaskConversionContext>`.
   */
  class TOGEOM_API GroupAction
      : public vrml_proc::action::ConversionContextAction<to_geom::conversion_context::MeshTaskConversionContext> {
   public:
    /**
     * @brief Properties for `GroupAction`. See VRML 2.0 specification for more information.
     */
    struct Properties {
      std::vector<std::shared_ptr<to_geom::conversion_context::MeshTaskConversionContext>> children;
    };
    /**
     * @brief Constructs a GroupAction with specified properties.
     * @param properties properties for GroupAction
     */
    GroupAction(Properties properties);

    /**
     * @brief The method is focused only on children field. All children are merged together and returned as a
     * pointer.
     * @return A shared pointer to the MeshTaskConversionContext.
     */
    std::shared_ptr<to_geom::conversion_context::MeshTaskConversionContext> Execute() override;

   private:
    Properties m_properties;
  };
}  // namespace to_geom::action
