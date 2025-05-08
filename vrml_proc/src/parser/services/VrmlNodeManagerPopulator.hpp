#pragma once

#include "VrmlNode.hpp"
#include "VrmlNodeManager.hpp"

#include "VrmlProcExport.hpp"

namespace vrml_proc::parser::VrmlNodeManagerPopulator {
  VRMLPROC_API void Populate(VrmlNodeManager& manager, const VrmlNode& node);
}
