#include "MemoryMappedFileReader.hpp"

#include <memory>
#include <string>

#include <boost/iostreams/device/mapped_file.hpp>

#include "Error.hpp"
#include "FileReader.hpp"
#include "FormatString.hpp"
#include "IoError.hpp"
#include "Logger.hpp"
#include "ManualTimer.hpp"

vrml_proc::core::io::MemoryMappedFile::MemoryMappedFile(const std::string& filepath)
    : m_mappedFile(filepath, boost::iostreams::mapped_file::readonly) {}

vrml_proc::core::io::FileReader<vrml_proc::core::io::MemoryMappedFile>::LoadFileResult
vrml_proc::core::io::MemoryMappedFileReader::Read(const std::filesystem::path& filepath) {  //

  using namespace vrml_proc::core::error;
  using namespace vrml_proc::core::io::error;
  using namespace vrml_proc::core::logger;
  using namespace vrml_proc::core::utils;

  LogInfo(FormatString("Read file <", filepath.string(), ">."), LOGGING_INFO);

  if (!std::filesystem::exists(filepath)) {
    LogError(FormatString("File <", filepath.string(), "> does not exits and thus cannot be read!"), LOGGING_INFO);
    std::shared_ptr<Error> error = std::make_shared<IoError>();
    return cpp::fail(error << (std::make_shared<FileNotFoundError>(filepath.string())));
  }

  vrml_proc::core::utils::ManualTimer timer;
  timer.Start();
  vrml_proc::core::io::MemoryMappedFile result(filepath.string());
  double time = timer.End();

  LogInfo(FormatString("Time to read and load file <", filepath.string(), "> of size ", result.GetSize(),
              " bytes into string took ", time, " seconds."),
      LOGGING_INFO);
  return result;
}
