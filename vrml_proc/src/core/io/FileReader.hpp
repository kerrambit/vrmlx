#pragma once

#include <string>
#include <filesystem>

#include <result.hpp>

#include "Error.hpp"

#include "VrmlProcExport.hpp"

namespace vrml_proc::core::io {
  /**
   * @brief Interface for reading data from a file into an object of type T.
   *
   * This abstract class defines a contract for file readers that load data
   * and return either the parsed result or an error. Implementations must define
   * the `Read()` method for reading specific file types or formats.
   *
   * @tparam T The type of the object that will be produced from the file.
   */
  template <typename T>
  class VRMLPROC_API FileReader {
   public:
    /**
     * @brief Represents a result of a file read operation.
     */
    using LoadFileResult = cpp::result<T, std::shared_ptr<vrml_proc::core::error::Error>>;

    /**
     * @brief Virtual destructor.
     */
    virtual ~FileReader() = default;

    /**
     * @brief Reads a file and attempts to load its contents into an object of type T.
     *
     * @param filepath path to the file to be read
     * @return result containing either the loaded object or an error
     */
    virtual LoadFileResult Read(const std::filesystem::path& filepath) = 0;
  };
}  // namespace vrml_proc::core::io
