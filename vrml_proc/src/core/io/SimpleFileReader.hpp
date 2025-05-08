#pragma once

#include <string>

#include "FileReader.hpp"

#include "VrmlProcExport.hpp"

namespace vrml_proc::core::io {
  class VRMLPROC_API SimpleFileReader : public FileReader<std::string> {
   public:
    LoadFileResult Read(const std::filesystem::path& filepath) override;
  };
}  // namespace vrml_proc::core::io
