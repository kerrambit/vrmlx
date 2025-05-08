#pragma once

#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>

#include "VrmlProcExport.hpp"

/**
 * @brief Helper macro, which can be used on all Log fucntions as the last paramater.
 */
#define LOGGING_INFO __FILE__, __LINE__, __FUNCTION__

/**
 * @brief Global logger instance for the application.
 *
 * This logger is used for logging messages with varying severity levels
 * throughout the application. It is implemented using Boost.Log and supports
 * multiple threads (via `severity_logger_mt`). The severity level can be controlled
 * through Boost's logging facilities, such as `boost::log::trivial::severity_level`.
 *
 * Example usage:
 * @code
 * BOOST_LOG_SEV(Logger::get(), boost::log::trivial::info) << "This is an info message";
 * @endcode
 */
BOOST_LOG_GLOBAL_LOGGER(Logger, boost::log::sources::severity_logger_mt<boost::log::trivial::severity_level>);

namespace vrml_proc::core::logger {
  /**
   * @brief Represents all logging levels.
   */
  enum class Level { Trace, Debug, Info, Warning, Error, Fatal };

  /**
   * @brief Initializes logging with default configuration.
   *
   * This version sets up logging with default settings.
   *
   * @note It should be called before any logging function is used. However, even before that, all log messages are
   * temporally stored and flusehd as soon as possible.
   */
  VRMLPROC_API void InitLogging();

  /**
   * @brief Initializes logging with custom directory and project name.
   *
   * @param loggingDirectory directory where log files will be written
   * @param projectName name of the project, used as a prefix for log files
   *
   * @note It should be called before any logging function is used. However, even before that, all log messages are
   * temporally stored and flusehd as soon as possible.
   */
  VRMLPROC_API void InitLogging(const std::string& loggingDirectory, const std::string& projectName);

  /**
   * @brief Logs a raw, unformatted message with a title.
   *
   * This is typically used for logging structured or long-form messages.
   *
   * @param title short descriptive title for the log entry
   * @param text main content of the log message
   * @param level severity level of the log (e.g., debug, info, warning)
   * @param file source file from which the log was emitted
   * @param line line number in the source file
   * @param function function name from which the log was emitted
   */
  VRMLPROC_API void LogUnformattedText(const std::string& title,
      const std::string& text,
      Level level,
      const std::string& file,
      int line,
      const std::string& function);

  /**
   * @brief Logs a formatted message.
   *
   * @param text message content to log
   * @param level severity level of the log (e.g., debug, info, error)
   * @param file source file from which the log was emitted
   * @param line line number in the source file
   * @param function function name from which the log was emitted
   */
  VRMLPROC_API void Log(
      const std::string& text, Level level, const std::string& file, int line, const std::string& function);

  /**
   * @brief Logs a trace-level message.
   *
   * @param text message content
   * @param file source file name
   * @param line line number where the log is called
   * @param function function name from which the log is called
   */
  VRMLPROC_API void LogTrace(const std::string& text, const std::string& file, int line, const std::string& function);

  /**
   * @brief Logs a debug-level message.
   *
   * @param text message content
   * @param file source file name
   * @param line line number where the log is called
   * @param function function name from which the log is called
   */
  VRMLPROC_API void LogDebug(const std::string& text, const std::string& file, int line, const std::string& function);

  /**
   * @brief Logs an info-level message.
   *
   * @param text message content
   * @param file source file name
   * @param line line number where the log is called
   * @param function function name from which the log is called*/
  VRMLPROC_API void LogInfo(const std::string& text, const std::string& file, int line, const std::string& function);

  /**
   * @brief Logs a warning-level message.
   *
   * @param text message content
   * @param file source file name
   * @param line line number where the log is called
   * @param function function name from which the log is called*/
  VRMLPROC_API void LogWarning(const std::string& text, const std::string& file, int line, const std::string& function);

  /**
   * @brief Logs an error-level message.
   *
   * @param text message content
   * @param file source file name
   * @param line line number where the log is called
   * @param function function name from which the log is called*/
  VRMLPROC_API void LogError(const std::string& text, const std::string& file, int line, const std::string& function);

  /**
   * @brief Logs a fatal-level message.
   *
   * @param text message content
   * @param file source file name
   * @param line line number where the log is called
   * @param function function name from which the log is called*/
  VRMLPROC_API void LogFatal(const std::string& text, const std::string& file, int line, const std::string& function);
}  // namespace vrml_proc::core::logger