#pragma once

#include <functional>
#include <memory>

#include "GeometryAction.hpp"
#include "Int32Array.hpp"
#include "MeshTaskConversionContext.hpp"
#include "VrmlNode.hpp"
#include "VrmlUnits.hpp"

#include "VrmlProcessingExport.hpp"

namespace to_geom {
  namespace action {

    class VRMLPROCESSING_API IndexedLineSetAction : public to_geom::action::GeometryAction {
     public:
      struct Properties {
        std::reference_wrapper<const vrml_proc::parser::VrmlNode> color;
        std::reference_wrapper<const vrml_proc::parser::VrmlNode> coord;
        std::reference_wrapper<const vrml_proc::parser::Int32Array> colorIndex;
        std::reference_wrapper<const bool> colorPerVertex;
        std::reference_wrapper<const vrml_proc::parser::Int32Array> coordIndex;
      };

      IndexedLineSetAction(Properties properties, GeometryAction::Properties geometryProperties);
      std::shared_ptr<to_geom::conversion_context::MeshTaskConversionContext> Execute() override;

     private:
      Properties m_properties;
    };
  }  // namespace action
}  // namespace to_geom
