#pragma once

#include <sstream>
#include <string>

namespace vrml_proc::core::utils {
  /**
   * @brief Utility function to create and format string from different arguments.

   * @note Inspired by https://stackoverflow.com/questions/58402766/fold-expression-with-strings-and-their-size.
   */
  template <typename... Args>
  std::string FormatString(const Args&... args) {
    std::ostringstream stream;
    (stream << ... << args);
    return stream.str();
  }
}  // namespace vrml_proc::core::utils
