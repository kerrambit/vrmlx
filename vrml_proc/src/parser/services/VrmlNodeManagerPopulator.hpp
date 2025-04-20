#pragma once

#include "VrmlNode.hpp"
#include "VrmlNodeManager.hpp"

#include "VrmlProcessingExport.hpp"

namespace vrml_proc::parser::VrmlNodeManagerPopulator {
  VRMLPROCESSING_API void Populate(VrmlNodeManager& manager, const VrmlNode& node);
}
