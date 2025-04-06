#pragma once

#include <sstream>

#include "Comparable.hpp"
#include "Object.hpp"
#include "TypeToString.hpp"

#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
  namespace core {
    namespace utils {

      template <contract::Comparable T>
      struct VRMLPROCESSING_API Range : public vrml_proc::core::Object {
        Range(T min, T max) : min(min), max(max) {}

        Range() : min(-std::numeric_limits<T>::infinity()), max(std::numeric_limits<T>::infinity()) {}

        void SetMinimum(T min) { this->min = min; }

        void SetMaximum(T max) { this->max = max; }

        bool CheckValueInRangeExclusive(T value) { return !(value <= min || value >= max); }

        bool CheckValueInRangeInclusive(T value) { return !(value < min || value > max); }

        std::string ToString() const override {
          std::ostringstream stream;
          stream << "Range<" << vrml_proc::core::utils::TypeToString<T>() << ">: { min: <" << min << ">, max: <" << max
                 << "> }";
          return stream.str();
        }

        T max;
        T min;
      };
    }  // namespace utils
  }    // namespace core
}  // namespace vrml_proc
