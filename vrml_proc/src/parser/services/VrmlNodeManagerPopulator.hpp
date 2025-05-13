#pragma once

#include "VrmlNode.hpp"
#include "VrmlNodeManager.hpp"

#include "VrmlProcExport.hpp"

namespace vrml_proc::parser::service::VrmlNodeManagerPopulator {
  /**
   * @brief Populates given VrmlNodeManager `manager` with all DEf-nodes found searching through given root `node`.
   */
  VRMLPROC_API void Populate(VrmlNodeManager& manager, const VrmlNode& node);
}  // namespace vrml_proc::parser::service::VrmlNodeManagerPopulator
