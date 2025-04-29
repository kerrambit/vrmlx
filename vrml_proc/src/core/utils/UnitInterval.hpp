#pragma once

#include <optional>

#include "Range.hpp"

namespace vrml_proc::core::utils {
  /**
   * @brief A type-safe class representing double value in <0.0, 1.0> interval.
   *
   * It provides utility methods such as conversion to percentage and obtaining the complementary value.
   */
  class UnitInterval {
   public:
    /**
     * @brief Attempts to create a UnitInterval from a given double value.
     *
     * @param value value to be validated and wrapped
     * @return valid instance if `value` is in <0.0, 1.0>, otherwise std::nullopt
     */
    static std::optional<UnitInterval> Create(double value) {
      static Range<double> range = Range(0.0, 1.0);
      if (!range.CheckValueInRangeInclusive(value)) {
        return {};
      }
      return UnitInterval(value);
    }

    /**
     * @brief Returns the underlying double value of the unit interval.
     *
     * @return raw value in <0.0, 1.0>
     */
    double GetValue() { return m_value; }

    /**
     * @brief Converts the unit interval value to a percentage.
     *
     * @return value in the range <0.0, 100.0> representing the percentage
     */
    double GetAsPercentage() { return m_value * 100; }

    /**
     * @brief Computes the complement of this unit interval.
     *
     * @return new UnitInterval with value (1.0 - this->m_value)
     */
    UnitInterval GetComplement() const { return UnitInterval(1.0 - m_value); }

   private:
    /**
     * @brief Constructs new object.
     *
     * @param value Must be in [0.0, 1.0].
     */
    explicit UnitInterval(double value) : m_value(value) {}

    double m_value;
  };
}  // namespace vrml_proc::core::utils
