#pragma once

#include <fstream>

#include <nlohmann/json.hpp>

#include "Error.hpp"
#include "FileReader.hpp"
#include "FormatString.hpp"
#include "IoError.hpp"
#include "Logger.hpp"
#include "ManualTimer.hpp"

namespace vrml_proc::core::io {
  /**
   * @brief Represents a reader for JSON files.
   */
  class JsonFileReader : public FileReader<nlohmann::json> {
   public:
    /**
     * @brief Reads a JSON file on `filepath`.
     *
     * @param filepath path to the JSON file
     * @returns read result type
     */
    LoadFileResult Read(const std::filesystem::path& filepath) override {
      using namespace vrml_proc::core::error;
      using namespace vrml_proc::core::io::error;
      using namespace vrml_proc::core::logger;
      using namespace vrml_proc::core::utils;

      LogInfo(FormatString("Read JSON file <", filepath.string(), ">."), LOGGING_INFO);

      if (!std::filesystem::exists(filepath)) {
        LogError(
            FormatString("JSON file <", filepath.string(), "> does not exits and thus cannot be read!"), LOGGING_INFO);
        std::shared_ptr<Error> error = std::make_shared<IoError>();
        return cpp::fail(error << (std::make_shared<FileNotFoundError>(filepath.string())));
      }

      vrml_proc::core::utils::ManualTimer timer;
      timer.Start();

      std::ifstream file(filepath.string());
      nlohmann::json config;
      file >> config;

      double time = timer.End();

      LogInfo(FormatString("Time to read and load JSON file <", filepath.string(), "> into json object took ", time,
                  " seconds."),
          LOGGING_INFO);
      return config;
    }
  };
}  // namespace vrml_proc::core::io
