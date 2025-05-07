#pragma once

#include <filesystem>
#include <memory>
#include <string>

#include <result.hpp>

#include <CGAL/boost/graph/IO/OBJ.h>

#include "Error.hpp"
#include "FileWriter.hpp"
#include "FormatString.hpp"
#include "IoError.hpp"
#include "Logger.hpp"
#include "Mesh.hpp"
#include "ScopedTimer.hpp"

namespace to_geom::core::io {

  class ObjFileWriter : public vrml_proc::core::io::FileWriter<to_geom::core::Mesh> {
   public:
    FileWriteResult Write(const std::filesystem::path& filepath, const to_geom::core::Mesh& data) override {
      using namespace vrml_proc::core::error;
      using namespace vrml_proc::core::io::error;
      using namespace vrml_proc::core::logger;
      using namespace vrml_proc::core::utils;

      LogInfo(FormatString("Write OBJ mesh into file <", filepath.string(), ">."), LOGGING_INFO);

      std::shared_ptr<Error> error = std::make_shared<IoError>();

      if (filepath.empty()) {
        return cpp::fail(error << (std::make_shared<EmptyFilePathError>(filepath.string())));
      }

      if (!std::filesystem::exists(filepath.parent_path())) {
        return cpp::fail(error << (std::make_shared<DirectoryNotFoundError>(filepath.parent_path().string())));
      }

      std::string ext = filepath.extension().string();
      if (ext != ".obj") {
        LogWarning(FormatString("You are about to write OBJ mesh into file with extension <", ext,
                       ">, which is different than expected <obj>!"),
            LOGGING_INFO);
      }

      double time;
      bool result;
      {
        vrml_proc::core::utils::ScopedTimer timer(time);
        result = CGAL::IO::write_OBJ(filepath.string(), data);
      }

      if (!result) {
        LogError(FormatString("Wrting of OBJ into file <", filepath.string(), "> was unsuccessful! Process took ", time,
                     " seconds."),
            LOGGING_INFO);
        return cpp::fail(error << (std::make_shared<GeneralWriteError>(filepath.string())));
      }

      LogInfo(FormatString(
                  "OBJ was successfully written into file <", filepath.string(), ">. Write took ", time, " seconds."),
          LOGGING_INFO);

      return {};
    }
  };
}  // namespace to_geom::core::io
