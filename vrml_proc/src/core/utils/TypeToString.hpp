#pragma once

#include <string>

#include <typeinfo>
#ifdef __GNUG__
#include <cxxabi.h>
#include <cstdlib>
#endif

namespace vrml_proc::core::utils {
  /**
   * @brief Utility function which returns a string representing the name of the type based on the passed const
   * reference to an entity.
   *
   * @note The __cxa_demangle function was found here
   * https://gcc.gnu.org/onlinedocs/libstdc++/libstdc++-html-USERS-4.3/a01696.html.
   */
  template <typename T>
  std::string TypeToString(const T& object) {
#ifdef __GNUG__
    int status;
    char* demangled = abi::__cxa_demangle(typeid(object).name(), nullptr, nullptr, &status);
    if (status == 0) {
      std::string result(demangled);
      std::free(demangled);
      return result;
    }
#endif
    return typeid(object).name();
  }

  /**
   * @brief Utility function which returns a string representing the name of the type based on the template typename
   * argument.
   * @note The __cxa_demangle function was found here
   * https://gcc.gnu.org/onlinedocs/libstdc++/libstdc++-html-USERS-4.3/a01696.html.
   */
  template <typename T>
  std::string TypeToString() {
#ifdef __GNUG__
    int status;
    char* demangled = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, &status);
    if (status == 0) {
      std::string result(demangled);
      std::free(demangled);
      return result;
    }
#endif
    return typeid(T).name();
  }
}  // namespace vrml_proc::core::utils
