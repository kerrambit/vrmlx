#pragma once

#include <concepts>
#include <type_traits>

namespace vrml_proc::core::contract {
  /**
   * @brief Concept that requires a type to support comparison operations.
   *
   * A type satisfies the Comparable concept if it supports both the less-than (`<`)
   * and greater-than (`>`) operators, and these operations return a value convertible to `bool`.
   *
   * @tparam T type to be tested for comparability
   */
  template <typename T>
  concept Comparable = requires(T a, T b) {
    { a < b } -> std::convertible_to<bool>;
    { a > b } -> std::convertible_to<bool>;
  };
}  // namespace vrml_proc::core::contract
