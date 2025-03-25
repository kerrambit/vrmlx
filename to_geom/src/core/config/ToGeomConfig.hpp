#pragma once

#include <filesystem>
#include <memory>
#include <string>

#include <result.hpp>

#include <nlohmann/json.hpp>

#include "Config.hpp"
#include "Error.hpp"
#include "ExportFormats.hpp"
#include "FormatString.hpp"
#include "JsonError.hpp"
#include "JsonFileReader.hpp"
#include "Logger.hpp"
#include "VrmlProcConfig.hpp"

namespace to_geom::core::config {

  /**
   * @brief Helper fucntion which converts string to ExportFormat and logs the process.
   *
   * @param string string to convert
   * @returns ExportFormat after conversion
   */
  inline to_geom::core::io::ExportFormat ConvertStringToExportFormat(const std::string& string) {
    using namespace to_geom::core::io;
    bool defaultUsed;
    auto format = ExportFormatUtils::StringToExportFormat(string, ExportFormat::Stl, defaultUsed);

    if (defaultUsed) {
      vrml_proc::core::logger::LogWarning(
          vrml_proc::core::utils::FormatString("While parsing JSON field <exportFormat>, an error occured. The value <",
              string, "> is not an expected value! The default value (<stl>) will be used."),
          LOGGING_INFO);
    }

    return format;
  }

  /**
   * @brief Represents a configuration file for `togeom` library.
   *
   * Configuration file for `togeom` has following properties:
   *  - exportFormat (ExportFormat),
   *  - vrmlProcConfig (inherited properties from VrmlProcConfig class).
   *
   * @implements `Config` class with Load() method.
   */
  struct ToGeomConfig : public vrml_proc::core::config::Config {
    /**
     * @brief Defaul constructor.
     */
    ToGeomConfig() = default;

    vrml_proc::core::config::VrmlProcConfig vrmlProcConfig;
    to_geom::core::io::ExportFormat exportFormat = to_geom::core::io::ExportFormat::Stl;

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
          vrmlProcConfig.ignoreUnknownNode = json.value().value("ignoreUnknownNode", false);
          exportFormat = ConvertStringToExportFormat(json.value().value("exportFormat", "stl"));
          vrmlProcConfig.logFileDirectory =
              json.value().value("logFileDirectory", std::filesystem::current_path().string());
          vrmlProcConfig.logFileName = json.value().value("logFileName", "vrmlproc");
        } catch (const nlohmann::json::exception& e) {
          return cpp::fail(std::make_shared<vrml_proc::core::error::JsonError>(e.what()));
        }

        return {};
      }

      return cpp::fail(json.error());
    }
  };
}  // namespace to_geom::core::config
