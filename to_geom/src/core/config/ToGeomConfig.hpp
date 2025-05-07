#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <thread>

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
#include "UnitInterval.hpp"

namespace to_geom::core::config {

  /**
   * @brief Helper function which converts string to ExportFormat and logs the process.
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
  struct ToGeomConfig : public vrml_proc::core::config::VrmlProcConfig {
    /**
     * @brief Represents settings object for IndexedFaceSet calculator.
     */
    struct IfsSettigs {
      bool checkRange = true;
      bool onlyTriangularFaces = false;
    };

    /**
     * @brief Represents settings for parallel computation of meshes.
     */
    struct ParallelismSettings {
      bool active = true;
      unsigned int threadsNumberLimit = std::thread::hardware_concurrency();
    };

    /**
     * @brief Represents settings for mesh simplification process.
     */
    struct MeshSimplificationSettings {
      bool active = false;
      vrml_proc::core::utils::UnitInterval percentageOfAllEdgesToSimplify =
          vrml_proc::core::utils::UnitInterval::Create(0.5).value();
    };

    /**
     * @brief Represents settings for an option object in export format.
     */
    struct ExportFormatOptions {
      bool binary = true;
    };

    /**
     * @brief Defaul constructor.
     */
    ToGeomConfig()
        : vrml_proc::core::config::VrmlProcConfig(),
          exportFormat(to_geom::core::io::ExportFormat::Stl),
          ifsSettings({true, false}),
          parallelismSettings({true, std::thread::hardware_concurrency()}) {}

    to_geom::core::io::ExportFormat exportFormat = to_geom::core::io::ExportFormat::Stl;
    ExportFormatOptions exportFormatOptions;
    IfsSettigs ifsSettings;
    ParallelismSettings parallelismSettings;
    MeshSimplificationSettings meshSimplificationSettings;

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
        auto loadBaseJson = LoadJson(json.value());
        if (loadBaseJson.has_error()) {
          return loadBaseJson;
        }
        try {
          if (json.value().contains("exportFormat") && (json.value())["exportFormat"].is_object()) {
            const auto& exportFormat = (json.value())["exportFormat"];
            this->exportFormat = ConvertStringToExportFormat(exportFormat.value("format", "stl"));
            if (exportFormat.contains("options") && exportFormat["options"].is_object()) {
              const auto& options = exportFormat["options"];
              exportFormatOptions.binary = options.value("binary", true);
            }
          }

          if (json.value().contains("IFSSettings") && (json.value())["IFSSettings"].is_object()) {
            const auto& ifs = (json.value())["IFSSettings"];
            ifsSettings.checkRange = ifs.value("checkRange", true);
            ifsSettings.onlyTriangularFaces = ifs.value("onlyTriangularFaces", false);
          }
          if (json.value().contains("parallelismSettings") && (json.value())["parallelismSettings"].is_object()) {
            const auto& parallelism = (json.value())["parallelismSettings"];
            parallelismSettings.active = parallelism.value("active", true);
            parallelismSettings.threadsNumberLimit =
                parallelism.value("threadsNumberLimit", std::thread::hardware_concurrency());
            if (parallelismSettings.threadsNumberLimit <= 0) {
              parallelismSettings.threadsNumberLimit = 1;
            }
          }
          if (json.value().contains("meshSimplification") && (json.value())["meshSimplification"].is_object()) {
            const auto& simplification = (json.value())["meshSimplification"];
            meshSimplificationSettings.active = simplification.value("active", true);
            double percentOfAllEdgesToSimplify = simplification.value("percentageOfAllEdgesToSimplify", 50.0);
            if (vrml_proc::core::utils::UnitInterval::Create(percentOfAllEdgesToSimplify / 100).has_value()) {
              meshSimplificationSettings.percentageOfAllEdgesToSimplify =
                  vrml_proc::core::utils::UnitInterval::Create(percentOfAllEdgesToSimplify / 100).value();
            } else {
              meshSimplificationSettings.percentageOfAllEdgesToSimplify =
                  vrml_proc::core::utils::UnitInterval::Create(0.5).value();
            }
          }
        } catch (const nlohmann::json::exception& e) {
          return cpp::fail(std::make_shared<vrml_proc::core::error::JsonError>(e.what()));
        }

        return {};
      }

      return cpp::fail(json.error());
    }
  };
}  // namespace to_geom::core::config
