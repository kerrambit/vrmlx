#pragma once

#include <cmath>
#include <cstdlib>

#include "Angle.hpp"

namespace vrml_proc::math {
  /**
   * @brief Class representing quaternion object. The quaternion is number system that extends the complex numbers.
   * Quaternions are used for calculations involving three-dimensional rotations,
   *
   * @note The main resource for this topic was https://www.cprogramming.com/tutorial/3d/quaternions.html.
   * @todo Values `x`, `y`, `z` and `w` should be private and getters and setters shouls be made!
   */
  struct Quaternion {
    /**
     * @brief Publicly available values. Note that value `w` is in radians!
     */
    double x, y, z, w;

    /**
     * @brief Constructs default quaternion.
     */
    Quaternion() : x(0.0), y(0.0), z(0.0), w(1.0) {}

    /**
     * @brief Constructs quaternion.
     *
     * @param x x value
     * @param y y value
     * @param z z value
     * @param angle w value representing angle
     */
    Quaternion(double x, double y, double z, const vrml_proc::math::Angle& angle) {
      double halfAngle = angle.GetValueAs(vrml_proc::math::Angle::AngleUnit::Radians) / 2.0;
      double sinOfHalfAngle = std::sin(halfAngle);

      this->w = std::cos(halfAngle);
      this->x = x * sinOfHalfAngle;
      this->y = y * sinOfHalfAngle;
      this->z = z * sinOfHalfAngle;
    }

    /**
     * @brief Overloaded operator for multiplying quaternions.
     */
    Quaternion operator*(const Quaternion& second) const {
      Quaternion result;
      result.x = (this->w * second.x + this->x * second.w + this->y * second.z - this->z * second.y);
      result.y = (this->w * second.y - this->x * second.z + this->y * second.w + this->z * second.x);
      result.z = (this->w * second.z + this->x * second.y - this->y * second.x + this->z * second.w);
      result.w = (this->w * second.w - this->x * second.x - this->y * second.y - this->z * second.z);

      return result;
    }

    /**
     * @brief Checks if the quaternion is normalized.
     *
     * @returns true if quaterniion is normalized, otherwise false
     */
    bool IsNormalized() const {
      const double epsilon = 1e-6;
      double norm = (x * x) + (y * y) + (z * z) + (w * w);
      return std::abs(norm - 1.0) < epsilon;
    }

    /**
     * @brief Normalized the quaternion.
     */
    void Normalize() {
      double norm = std::sqrt(x * x + y * y + z * z + w * w);
      if (norm > 0.0) {
        x /= norm;
        y /= norm;
        z /= norm;
        w /= norm;
      }
    }

    /**
     * @brief Inverses the quaternion.
     * @warning Make sure that quaternion is not zero-norm!
     *
     * @note Link: https://www.mathworks.com/help/aeroblks/quaternioninverse.html.
     */
    void Inverse() {
      double scalarDivisor = (x * x) + (y * y) + (z * z) + (w * w);

      x = (-1 * x) / scalarDivisor;
      y = (-1 * y) / scalarDivisor;
      z = (-1 * z) / scalarDivisor;
      w = w / scalarDivisor;
    }
  };
}  // namespace vrml_proc::math
