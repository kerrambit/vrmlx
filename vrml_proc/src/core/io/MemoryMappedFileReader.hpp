#pragma once

#include <string>
#include <memory>

#include <boost/iostreams/device/mapped_file.hpp>

#include "FileReader.hpp"

#include "VrmlProcExport.hpp"

namespace vrml_proc::core::io {

  struct MemoryMappedFile {
    MemoryMappedFile(const std::string& filepath);

    const char* GetBegin() const { return m_mappedFile.const_data(); }
    const char* GetEnd() const { return m_mappedFile.const_data() + m_mappedFile.size(); }
    size_t GetSize() const { return m_mappedFile.size(); }

   private:
    boost::iostreams::mapped_file m_mappedFile;
  };

  class VRMLPROC_API MemoryMappedFileReader : public FileReader<MemoryMappedFile> {
   public:
    LoadFileResult Read(const std::filesystem::path& filepath) override;
  };
}  // namespace vrml_proc::core::io
