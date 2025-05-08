#pragma once

#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>

#include "VrmlProcExport.hpp"

#define LOGGING_INFO __FILE__, __LINE__, __FUNCTION__

BOOST_LOG_GLOBAL_LOGGER(Logger, boost::log::sources::severity_logger_mt<boost::log::trivial::severity_level>)

namespace vrml_proc::core::logger {
  enum class Level { Trace, Debug, Info, Warning, Error, Fatal };
  VRMLPROC_API void InitLogging();
  VRMLPROC_API void InitLogging(const std::string& loggingDirectory, const std::string& projectName);
  VRMLPROC_API void LogUnformattedText(const std::string& title,
      const std::string& text,
      Level level,
      const std::string& file,
      int line,
      const std::string& function);
  VRMLPROC_API void Log(
      const std::string& text, Level level, const std::string& file, int line, const std::string& function);
  VRMLPROC_API void LogTrace(
      const std::string& text, const std::string& file, int line, const std::string& function);
  VRMLPROC_API void LogDebug(
      const std::string& text, const std::string& file, int line, const std::string& function);
  VRMLPROC_API void LogInfo(
      const std::string& text, const std::string& file, int line, const std::string& function);
  VRMLPROC_API void LogWarning(
      const std::string& text, const std::string& file, int line, const std::string& function);
  VRMLPROC_API void LogError(
      const std::string& text, const std::string& file, int line, const std::string& function);
  VRMLPROC_API void LogFatal(
      const std::string& text, const std::string& file, int line, const std::string& function);
}  // namespace vrml_proc::core::logger