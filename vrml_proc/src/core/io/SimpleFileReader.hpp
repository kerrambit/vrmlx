#pragma once

#include <string>

#include "FileReader.hpp"

#include "VrmlProcExport.hpp"

namespace vrml_proc::core::io {
  /**
   * @brief Basic file reader that loads the entire file content into a string.
   *
   * This implementation of FileReader reads the full contents of a file into a
   * standard `std::string`. It is suitable for reading text-based files where
   * no special parsing or memory mapping is required.
   */
  class VRMLPROC_API SimpleFileReader : public FileReader<std::string> {
   public:
    /**
     * @brief Reads the content of the file at the specified path into a string.
     *
     * @param filepath path to the file to be read
     * @return result containing the file content as a string, or an error on failure
     */
    LoadFileResult Read(const std::filesystem::path& filepath) override;
  };
}  // namespace vrml_proc::core::io
