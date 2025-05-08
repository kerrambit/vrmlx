#pragma once

#include <filesystem>
#include <memory>

#include <result.hpp>

#include "Error.hpp"

#include "VrmlProcExport.hpp"

namespace vrml_proc::core::io {

  template <typename T>
  class VRMLPROC_API FileWriter {
   public:
    using FileWriteResult = cpp::result<void, std::shared_ptr<vrml_proc::core::error::Error>>;

    virtual ~FileWriter() = default;

    virtual FileWriteResult Write(const std::filesystem::path& filepath, const T& data) = 0;
  };
}  // namespace vrml_proc::core::io
