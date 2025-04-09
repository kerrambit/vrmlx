#pragma once

#include <utility>

#include "VrmlUnits.hpp"

namespace vrml_proc::parser {
  struct Vec2f {
    Vec2f() : u(0.0f), v(0.0f) {}

    Vec2f(float32_t u, float32_t v) : u(u), v(v) {}

    Vec2f(Vec2f&& other) noexcept : u(std::exchange(other.u, 0.0f)), v(std::exchange(other.v, 0.0f)) {}

    Vec2f& operator=(Vec2f&& other) noexcept {
      if (this != &other) {
        u = std::exchange(other.u, 0.0f);
        v = std::exchange(other.v, 0.0f);
      }
      return *this;
    }

    Vec2f(const Vec2f&) = default;

    Vec2f& operator=(const Vec2f&) = default;

    float32_t u;
    float32_t v;
  };
}  // namespace vrml_proc::parser
