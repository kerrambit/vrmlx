#pragma once

#include <numbers>

namespace vrml_proc::math {

  /**
   * @brief Represents an angle object.
   */
  class Angle {
   public:
    /**
     * @brief Different angle units.
     */
    enum class AngleUnit { Degree, Radians };

    /**
     * @brief Constructs an angle.
     *
     * @param unit unit used in `value` paramater
     * @param value value of the angle in `unit`
     */
    Angle(AngleUnit unit, double value) : m_unit(unit), m_value(value) {}

    /**
     * @brief Converts the stored angle value to the specified unit.
     *
     * @param unit target angle unit to convert the value to (either `AngleUnit::Degree` or `AngleUnit::Radian`)
     * @return angle value in the specified unit
     */
    double GetValueAs(AngleUnit unit) const {
      if (unit == m_unit) {
        return m_value;
      }
      if (unit == AngleUnit::Degree) {
        return m_value * (180.0 / std::numbers::pi_v<double>);
      } else {
        return m_value * (std::numbers::pi_v<double> / 180.0);
      }
    }

    /**
     * @brief Gets the current angle unit.
     *
     * @return current angle unit (`AngleUnit::Degree` or `AngleUnit::Radian`)
     */
    AngleUnit GetUnit() const { return m_unit; }

    /**
     * @brief Sets new angle value and its unit.
     *
     * @param unit unit of the angle (either `AngleUnit::Degree` or `AngleUnit::Radian`)
     * @param value new angle value in the specified unit
     */
    void SetValue(AngleUnit unit, double value) {
      m_unit = unit;
      m_value = value;
    }

   private:
    AngleUnit m_unit;
    double m_value;
  };
}  // namespace vrml_proc::math
