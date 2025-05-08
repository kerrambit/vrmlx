#pragma once

#include "Error.hpp"

namespace vrml_proc::core::config {
  /**
   * @brief Config is an interface for representing a generic configuration file.
   */
  struct Config {
    virtual ~Config() = default;
    /**
     * @brief Loads a configuration file from the given file path.
     *
     * @param filepath path to the file to load
     * @return result containing error on failure, else empty
     */
    virtual cpp::result<void, std::shared_ptr<vrml_proc::core::error::Error>> Load(
        const std::filesystem::path& filepath) = 0;
  };
}  // namespace vrml_proc::core::config
