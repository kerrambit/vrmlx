#pragma once

#include <filesystem>
#include <memory>

#include <result.hpp>

#include <nlohmann/json.hpp>

#include "Error.hpp"
#include "JsonError.hpp"
#include "JsonFileReader.hpp"
#include "Config.hpp"

namespace vrml_proc::core::config {
  /**
   * @brief Represents a configuration file for `vrmlproc` library.
   *
   * Configuration file for `vrmlproc` has following properties:
   *  - ignoreUnknownNode (bool),
   *  - logFileDirectory (string),
   *  - logFileName (string).
   *
   * @implements `Config` class with Load() method.
   */
  struct VrmlProcConfig : public Config {
    /**
     * @brief Defaul constructor.
     */
    VrmlProcConfig() = default;

    bool ignoreUnknownNode = false;
    std::string logFileDirectory = std::filesystem::current_path().string();
    std::string logFileName = "vrmlproc";

    /**
     * @brief Loads configuration file from JSON file.
     *
     * @param filepath path to the file to load
     * @return result containing error on failure, else empty
     */
    cpp::result<void, std::shared_ptr<vrml_proc::core::error::Error>> Load(
        const std::filesystem::path& filepath) override {
      vrml_proc::core::io::JsonFileReader reader;
      auto json = reader.Read(filepath);

      if (json.has_value()) {
        try {
          ignoreUnknownNode = json.value().value("ignoreUnknownNode", false);
          logFileDirectory = json.value().value("logFileDirectory", std::filesystem::current_path().string());
          logFileName = json.value().value("logFileName", "vrmlproc");
        } catch (const nlohmann::json::exception& e) {
          return cpp::fail(std::make_shared<vrml_proc::core::error::JsonError>(e.what()));
        }

        return {};
      }

      return cpp::fail(json.error());
    }
  };
}  // namespace vrml_proc::core::config
