#pragma once

#include <numbers>

namespace vrml_proc::math {
  class Angle {
   public:
    enum class AngleUnit { Degree, Radians };

    Angle(AngleUnit unit, double value) : m_unit(unit), m_value(value) {}

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

    AngleUnit GetUnit() const { return m_unit; }

    void SetValue(AngleUnit unit, double value) {
      m_unit = unit;
      m_value = value;
    }

   private:
    AngleUnit m_unit;
    double m_value;
  };
}  // namespace vrml_proc::math
