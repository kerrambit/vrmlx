#pragma once

#include <string>
#include <memory>

#include <boost/iostreams/device/mapped_file.hpp>

#include "FileReader.hpp"

#include "VrmlProcExport.hpp"

namespace vrml_proc::core::io {

  /**
   * @brief Represents memory mapped file.
   *
   * It contains pointers to the beginning and end of the file content.
   */
  struct MemoryMappedFile {
    /**
     * @brief Constructs a enw memory mapped file on location `filepath`.
     *
     * By creating this instancem, a file is mapped to thd ememory and this instance will provide points to its content.
     *
     * @param filepath path to the file
     */
    MemoryMappedFile(const std::string& filepath);

    /**
     * @brief Retrieve a pointer to the start of file's content.
     *
     * @returns begin pointer
     */
    const char* GetBegin() const { return m_mappedFile.const_data(); }

    /**
     * @brief Retrieve a pointer to the end of file's content.
     *
     * @returns end pointer
     */
    const char* GetEnd() const { return m_mappedFile.const_data() + m_mappedFile.size(); }

    /**
     * @brief Gets a total size of the file in bytes.
     *
     * @returns size of file in bytes
     */
    size_t GetSize() const { return m_mappedFile.size(); }

   private:
    boost::iostreams::mapped_file m_mappedFile;
  };

  /**
   * @brief File reader implementation using memory-mapped file access.
   *
   * This class provides an efficient way to read files by mapping them into memory,
   * avoiding unnecessary copying of file contents. It implements the FileReader interface
   * for the MemoryMappedFile type.
   */
  class VRMLPROC_API MemoryMappedFileReader : public FileReader<MemoryMappedFile> {
   public:
    /**
     * @brief Reads a file at the specified path using a memory-mapped approach.
     *
     * @param filepath The path to the file to be memory-mapped.
     * @return A result containing the memory-mapped file or an error on failure.
     */
    LoadFileResult Read(const std::filesystem::path& filepath) override;
  };
}  // namespace vrml_proc::core::io
