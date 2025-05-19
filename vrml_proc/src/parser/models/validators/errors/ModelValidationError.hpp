#pragma once

#include <sstream>
#include <string>

#include "Comparable.hpp"
#include "Error.hpp"
#include "Range.hpp"
#include "TypeToString.hpp"
#include "Vec3f.hpp"
#include "VrmlUnits.hpp"

namespace vrml_proc::parser::model::validator::error {
  /**
   * @brief Represents a generic model validation error.
   */
  class ModelValidationError : public core::error::Error {
   protected:
    virtual std::string GetMessageInternal() const { return "[ModelValidationError]"; }
  };

  /**
   * @brief Error for an empty array.
   */
  class EmptyArrayError : public ModelValidationError {
   public:
    EmptyArrayError(std::string name) : m_name(name) {}

   protected:
    std::string GetMessageInternal() const override {
      using core::utils::TypeToString;

      std::ostringstream oss;
      oss << ModelValidationError::GetMessageInternal() << "[EmptyArrayError]: array <" << m_name
          << "> cannot be empty!\n";

      return oss.str();
    }

   private:
    std::string m_name;
  };

  /**
   * @brief Error for number out of range.
   */
  template <core::contract::Comparable T>
  class NumberOutOfRangeError : public ModelValidationError {
   public:
    NumberOutOfRangeError(core::utils::Range<T> expectedRange, T actual)
        : m_expectedRange(expectedRange), m_actual(actual) {}

   protected:
    std::string GetMessageInternal() const override {
      using core::utils::TypeToString;

      std::ostringstream oss;
      oss << ModelValidationError::GetMessageInternal() << "[NumberOutOfRangeError]: Entity of type <"
          << TypeToString(m_actual) << "> holding value <" << m_actual << "> is invalid. Expected range is "
          << m_expectedRange << "!\n";

      return oss.str();
    }

   private:
    core::utils::Range<T> m_expectedRange;
    T m_actual;
  };

  /**
   * @brief Error for vec3f out of range.
   */
  class Vec3fOutOfRangeError : public ModelValidationError {
   public:
    Vec3fOutOfRangeError(core::utils::Range<float32_t> expectedRange, Vec3f vector)
        : m_expectedRange(expectedRange), m_vector(vector) {}

   protected:
    std::string GetMessageInternal() const override {
      using core::utils::TypeToString;

      std::ostringstream oss;
      oss << ModelValidationError::GetMessageInternal()
          << "[Vec3fOutOfRangeError]: one (or possibly more) of the vector's components are in invalid range! Actual "
             "vector is "
          << m_vector << ". Expected range is " << m_expectedRange << "!\n";
      return oss.str();
    }

   private:
    core::utils::Range<vrml_proc::parser::model::float32_t> m_expectedRange;
    Vec3f m_vector;
  };
}  // namespace vrml_proc::parser::model::validator::error
