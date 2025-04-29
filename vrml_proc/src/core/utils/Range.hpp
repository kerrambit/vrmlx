#pragma once

#include <sstream>

#include "Comparable.hpp"
#include "Object.hpp"
#include "TypeToString.hpp"

namespace vrml_proc::core::utils {
  /**
   * @brief Represents a range object with its minimum and maximum values.
   *
   * @tparam T is must be comparable
   *
   * @todo Use Rust create() pattern here to make checks that max is not greater than min.
   */
  template <contract::Comparable T>
  class Range : public vrml_proc::core::Object {
   public:
    /**
     * @brief Constructs new range with its min and max values.
     *
     * Make sure to pass valid values: where min <= max!
     *
     * @param min minimal value of the range
     * @param m_max m_maximum value of the range
     */
    Range(T min, T max) : m_min(min), m_max(max) {}

    /**
     * @brief Constructs new range with its smallest and gratest values.
     */
    Range() : m_min(-std::numeric_limits<T>::infinity()), m_max(std::numeric_limits<T>::infinity()) {}

    /**
     * @brief Sets new minimum of the range.
     *
     * @param min min value
     */
    void SetMinimum(T min) { m_min = min; }

    /**
     * @brief Sets new maximum of the range.
     *
     * @param max max value
     */
    void SetMaximum(T max) { m_max = max; }

    /**
     * @brief Returns current min.
     *
     * @returns value of lower bound - min
     */
    T GetMinimum() const { return m_min; }

    /**
     * @brief Returns current max.
     *
     * @returns value of higher bound - max
     */
    T GetMaximum() const { return m_max; }

    /**
     * @brief Checks that value is belongs to open range (min, max);
     *
     * @param value value to check
     * @returns true if value is in range
     */
    bool CheckValueInRangeExclusive(T value) const { return !(value <= m_min || value >= m_max); }

    /**
     * @brief Checks that value is belongs to closed range <min, max>;
     *
     * @param value value to check
     * @returns true if value is in range
     */
    bool CheckValueInRangeInclusive(T value) const { return !(value < m_min || value > m_max); }

    /**
     * @copydoc Object::ToString
     */
    std::string ToString() const override {
      std::ostringstream stream;
      stream << "Range<" << vrml_proc::core::utils::TypeToString<T>() << ">: { min: <" << m_min << ">, max: <" << m_max
             << "> }";
      return stream.str();
    }

   private:
    T m_max;
    T m_min;
  };
}  // namespace vrml_proc::core::utils
