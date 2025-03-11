#pragma once

#include <cstdint>
#include <string_view>

namespace vrml_proc::core::utils {

  /**
   * @brief Returns hash of the given string.
   * @param string string to hash
   * @note Source: https://stackoverflow.com/questions/650162/why-cant-the-switch-statement-be-applied-to-strings.
   * @returns hash code of the string
   */
  inline constexpr uint32_t Hash(std::string_view string) noexcept {
    uint32_t hash = 5381;
    for (char c : string) hash = ((hash << 5) + hash) + static_cast<uint8_t>(c);
    return hash;
  }
}  // namespace vrml_proc::core::utils
