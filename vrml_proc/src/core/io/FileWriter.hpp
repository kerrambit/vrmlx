#pragma once

#include <filesystem>
#include <memory>

#include <result.hpp>

#include "Error.hpp"

#include "VrmlProcExport.hpp"

namespace vrml_proc::core::io {
  /**
   * @brief Interface for writing an object of type T to a file.
   *
   * This abstract class defines a contract for file writers that serialize or
   * export data to a file. Implementations must define the `Write()` method
   * to handle specific formats or file types.
   *
   * @tparam T The type of the object that will be written to the file.
   */
  template <typename T>
  class VRMLPROC_API FileWriter {
   public:
    /**
     * @brief Represents a result of a file write operation.
     */
    using FileWriteResult = cpp::result<void, std::shared_ptr<vrml_proc::core::error::Error>>;

    /**
     * @brief Virtual destructor.
     */
    virtual ~FileWriter() = default;

    /**
     * @brief Writes the provided data to the specified file path.
     *
     * @param filepath path to the file where the data will be written
     * @param data object of type T to write to the file
     * @return result indicating success or an error describing the failure
     */
    virtual FileWriteResult Write(const std::filesystem::path& filepath, const T& data) = 0;
  };

}  // namespace vrml_proc::core::io
