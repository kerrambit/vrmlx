#pragma once

#include <iostream>
#include <utility>

#include "Printable.hpp"
#include "VrmlProcessingExport.hpp"
#include "VrmlUnits.hpp"

namespace vrml_proc {
  namespace parser {

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

      friend std::ostream& operator<<(std::ostream& stream, const Vec3f& vector) {
        return stream << "Vec3f: { x: <" << vector.x << ">, y: <" << vector.y << ">, z: <" << vector.z << "> }";
      }

      float32_t x = 0.0f;
      float32_t y = 0.0f;
      float32_t z = 0.0f;
    };
  }  // namespace parser
}  // namespace vrml_proc
