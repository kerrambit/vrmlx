#pragma once

#include "TransformationMatrix.hpp"
#include "VrmlNode.hpp"
#include "VrmlNodeManager.hpp"
#include "VrmlProcConfig.hpp"

#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
  namespace traversor {

    struct VRMLPROCESSING_API VrmlNodeTraversorParameters {
      VrmlNodeTraversorParameters(const vrml_proc::parser::VrmlNode& node,
                                  const vrml_proc::parser::VrmlNodeManager& manager, bool isDescendantOfShape,
                                  vrml_proc::math::TransformationMatrix transformation,
                                  const vrml_proc::core::config::VrmlProcConfig& config)
          : node(node),
            manager(manager),
            IsDescendantOfShape(isDescendantOfShape),
            transformation(transformation),
            config(config) {}

      const vrml_proc::parser::VrmlNode& node;
      const vrml_proc::parser::VrmlNodeManager& manager;
      bool IsDescendantOfShape;
      vrml_proc::math::TransformationMatrix transformation;
      const vrml_proc::core::config::VrmlProcConfig& config;
    };
  }  // namespace traversor
}  // namespace vrml_proc
