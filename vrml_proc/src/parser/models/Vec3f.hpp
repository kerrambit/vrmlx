#pragma once

#include <iostream>
#include <sstream>
#include <utility>

#include "Object.hpp"
#include "VrmlUnits.hpp"

namespace vrml_proc::parser {
  struct Vec3f : public vrml_proc::core::Object {
    Vec3f() : x(0.0f), y(0.0f), z(0.0f) {}

    Vec3f(float32_t x, float32_t y, float32_t z) : x(x), y(y), z(z) {}

    Vec3f(Vec3f&& other) noexcept
        : x(std::exchange(other.x, 0.0f)), y(std::exchange(other.y, 0.0f)), z(std::exchange(other.z, 0.0f)) {}

    Vec3f& operator=(Vec3f&& other) noexcept {
      if (this != &other) {
        x = std::exchange(other.x, 0.0f);
        y = std::exchange(other.y, 0.0f);
        z = std::exchange(other.z, 0.0f);
      }
      return *this;
    }

    Vec3f(const Vec3f&) = default;

    Vec3f& operator=(const Vec3f&) = default;

    std::string ToString() const override {
      std::ostringstream stream;
      stream << "Vec3f: { x: <" << x << ">, y: <" << y << ">, z: <" << z << "> }";
      return stream.str();
    }

    float32_t x;
    float32_t y;
    float32_t z;
  };
}  // namespace vrml_proc::parser
