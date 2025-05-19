#pragma once

#include <filesystem>
#include <memory>
#include <string>

#include <result.hpp>

#include <CGAL/boost/graph/IO/PLY.h>

#include "Error.hpp"
#include "FileWriter.hpp"
#include "FormatString.hpp"
#include "IoError.hpp"
#include "Logger.hpp"
#include "Mesh.hpp"
#include "ScopedTimer.hpp"

namespace to_geom::core::io {
  /**
   * @brief Represents a file writer for PLY format.
   */
  class PlyFileWriter : public vrml_proc::core::io::FileWriter<to_geom::core::Mesh> {
   public:
    /**
     * @brief Constructs new object. It sets up a flag indicating if the file should be written in binary PLY format to
     * true.
     */
    PlyFileWriter() : m_binaryMode(true) {}

    /**
     * @brief Constructs new object.
     *
     * @param binaryMode boolean flag indicating if the file should be written in binary PLY format
     */
    PlyFileWriter(bool binaryMode) : m_binaryMode(binaryMode) {}

    /**
     * @brief Inheritted method from `FileWriter`. It writes mesh in `data` into file on `filepath`.
     */
    FileWriteResult Write(const std::filesystem::path& filepath, const to_geom::core::Mesh& data) override {  //

      using namespace vrml_proc::core::error;
      using namespace vrml_proc::core::io::error;
      using namespace vrml_proc::core::logger;
      using namespace vrml_proc::core::utils;

      LogInfo(FormatString("Write PLY mesh into file <", filepath.string(), ">."), LOGGING_INFO);

      std::shared_ptr<Error> error = std::make_shared<IoError>();

      if (filepath.empty()) {
        return cpp::fail(error << (std::make_shared<EmptyFilePathError>(filepath.string())));
      }

      if (!std::filesystem::exists(filepath.parent_path())) {
        return cpp::fail(error << (std::make_shared<DirectoryNotFoundError>(filepath.parent_path().string())));
      }

      std::string ext = filepath.extension().string();
      if (ext != ".ply") {
        LogWarning(FormatString("You are about to write PLY mesh into file with extension <", ext,
                       ">, which is different than expected <ply>!"),
            LOGGING_INFO);
      }

      double time;
      bool result;
      {
        vrml_proc::core::utils::ScopedTimer timer(time);
        result = CGAL::IO::write_PLY(filepath.string(), data, CGAL::parameters::use_binary_mode(m_binaryMode));
      }

      if (!result) {
        LogError(FormatString("Wrting of PLY into file <", filepath.string(), "> was unsuccessful! Process took ", time,
                     " seconds."),
            LOGGING_INFO);
        return cpp::fail(error << (std::make_shared<GeneralWriteError>(filepath.string())));
      }

      LogInfo(FormatString(
                  "PLY was successfully written into file <", filepath.string(), ">. Write took ", time, " seconds."),
          LOGGING_INFO);

      return {};
    }

   private:
    bool m_binaryMode;
  };
}  // namespace to_geom::core::io
