#pragma once

#include "VrmlFile.hpp"
#include "VrmlNodeManager.hpp"
#include "VrmlProcConfig.hpp"

namespace vrml_proc::traversor {
  /**
   * @brief Represents parameters passed to the VrmlFileTraversor.
   */
  struct VrmlFileTraversorParameters {
    /**
     * @brief Constructs new object.
     *
     * @param file VRML file
     * @param manager manager to resolve USE nodes
     * @param config configuration file
     */
    VrmlFileTraversorParameters(const vrml_proc::parser::VrmlFile& file,
        const vrml_proc::parser::VrmlNodeManager& manager,
        const vrml_proc::core::config::VrmlProcConfig& config)
        : file(file), manager(manager), config(config) {}

    const vrml_proc::parser::VrmlFile& file;
    const vrml_proc::parser::VrmlNodeManager& manager;
    const vrml_proc::core::config::VrmlProcConfig& config;
  };
}  // namespace vrml_proc::traversor
