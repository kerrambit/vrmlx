#pragma once

#include "VrmlFile.hpp"
#include "VrmlNodeManager.hpp"
#include "VrmlProcConfig.hpp"

#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
  namespace traversor {

    struct VRMLPROCESSING_API VrmlFileTraversorParameters {
      VrmlFileTraversorParameters(const vrml_proc::parser::VrmlFile& file,
                                  const vrml_proc::parser::VrmlNodeManager& manager,
                                  const vrml_proc::core::config::VrmlProcConfig& config)
          : file(file), manager(manager), config(config) {}

      const vrml_proc::parser::VrmlFile& file;
      const vrml_proc::parser::VrmlNodeManager& manager;
      const vrml_proc::core::config::VrmlProcConfig& config;
    };
  }  // namespace traversor
}  // namespace vrml_proc
