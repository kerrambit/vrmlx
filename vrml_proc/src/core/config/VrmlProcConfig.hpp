#pragma once

#include <filesystem>
#include <memory>

#include <result.hpp>

#include <nlohmann/json.hpp>

#include "Config.hpp"
#include "Error.hpp"
#include "JsonError.hpp"
#include "JsonFileReader.hpp"

namespace vrml_proc::core::config {
  /**
   * @brief Represents a configuration file for `vrmlproc` library.
   *
   * Configuration file for `vrmlproc` has following properties:
   *  - ignoreUnknownNode (bool),
   *  - logFileDirectory (string),
   *  - logFileName (string),
   *  - synonymsFile (string).
   *
   * @implements `Config` class with Load() method.
   */
  struct VrmlProcConfig : public Config {
    virtual ~VrmlProcConfig() = default;

    /**
     * @brief Defaul constructor.
     */
    VrmlProcConfig()
        : ignoreUnknownNode(false),
          logFileDirectory(std::filesystem::current_path().string()),
          logFileName("vrmlproc"),
          synonymsFile((std::filesystem::current_path() / std::filesystem::path("vrmlprocSynonyms.json")).string()) {}

    VrmlProcConfig(bool ignoreUnknownNode, const std::string& logFileDirectory, const std::string& logFileName)
        : ignoreUnknownNode(ignoreUnknownNode),
          logFileDirectory(logFileDirectory),
          logFileName(logFileName),
          synonymsFile((std::filesystem::current_path() / std::filesystem::path("vrmlprocSynonyms.json")).string()){};

    VrmlProcConfig(bool ignoreUnknownNode,
        const std::string& logFileDirectory,
        const std::string& logFileName,
        const std::string& synonymsFile)
        : ignoreUnknownNode(ignoreUnknownNode),
          logFileDirectory(logFileDirectory),
          logFileName(logFileName),
          synonymsFile(synonymsFile){};

    bool ignoreUnknownNode = false;
    std::string logFileDirectory = std::filesystem::current_path().string();
    std::string logFileName = "vrmlproc";
    std::string synonymsFile =
        (std::filesystem::current_path() / std::filesystem::path("vrmlprocSynonyms.json")).string();

    /**
     * @brief Loads configuration file from JSON file.
     *
     * @param filepath path to the file to load
     * @return result containing error on failure, else empty
     */
    cpp::result<void, std::shared_ptr<vrml_proc::core::error::Error>> Load(
        const std::filesystem::path& filepath) override {  //

      vrml_proc::core::io::JsonFileReader reader;
      auto json = reader.Read(filepath);

      if (json.has_value()) {
        return LoadJson(json.value());
      }

      return cpp::fail(json.error());
    }

   protected:
    cpp::result<void, std::shared_ptr<vrml_proc::core::error::Error>> LoadJson(const nlohmann::json& json) {
      try {
        ignoreUnknownNode = json.value("ignoreUnknownNode", false);
        logFileDirectory = json.value("logFileDirectory", std::filesystem::current_path().string());
        logFileName = json.value("logFileName", "vrmlproc");
        synonymsFile = json.value("synonymsFile", std::filesystem::current_path().string());
      } catch (const nlohmann::json::exception& e) {
        return cpp::fail(std::make_shared<vrml_proc::core::io::error::JsonError>(e.what()));
      }
      return {};
    }
  };
}  // namespace vrml_proc::core::config
