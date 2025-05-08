#pragma once

#include <string>
#include <filesystem>

#include <result.hpp>

#include "Error.hpp"

#include "VrmlProcExport.hpp"

namespace vrml_proc::core::io {

  template <typename T>
  class VRMLPROC_API FileReader {
   public:
    using LoadFileResult = cpp::result<T, std::shared_ptr<vrml_proc::core::error::Error>>;

    virtual ~FileReader() = default;

    virtual LoadFileResult Read(const std::filesystem::path& filepath) = 0;
  };
}  // namespace vrml_proc::core::io
