#pragma once

#include <memory>
#include <sstream>
#include <string>

#include "Error.hpp"

namespace to_geom::calculator::error {

  class CalculatorError : public vrml_proc::core::error::Error {
   protected:
    virtual std::string GetMessageInternal() const { return "[CalculatorError]"; }
  };

  class PropertiesError : public vrml_proc::core::error::Error {
   public:
    PropertiesError(std::shared_ptr<vrml_proc::core::error::Error> innerError) { SetInnerError(innerError); }
    PropertiesError() = default;

   protected:
    virtual std::string GetMessageInternal() const {
      return "[PropertiesError]: some of the properties passed to the calculator are not valid!\n";
    }
  };

  class InvalidNumberOfCoordinatesForFaceError : public vrml_proc::core::error::Error {
   public:
    InvalidNumberOfCoordinatesForFaceError(size_t numberOfCoordinates) : m_numberOfCoordinates(numberOfCoordinates) {}

   protected:
    virtual std::string GetMessageInternal() const {
      std::ostringstream stream;
      stream
          << "[InvalidNumberOfCoordinatesForFaceError]: minimal number of coordinates to form a face is 3! The actual "
             "number received is <"
          << m_numberOfCoordinates << ">.\n";
      return stream.str();
    }

   private:
    size_t m_numberOfCoordinates;
  };

  class VertexIndexOutOfRangeError : public vrml_proc::core::error::Error {
   public:
    VertexIndexOutOfRangeError(std::shared_ptr<vrml_proc::core::error::Error> innerError) { SetInnerError(innerError); }
    VertexIndexOutOfRangeError() = default;

   protected:
    virtual std::string GetMessageInternal() const {
      std::ostringstream stream;
      stream << "[VertexIndexOutOfRangeError]: vertex index must point to valid index inside coordinates array!\n";
      return stream.str();
    }
  };

  class BoxCalculatorError : public CalculatorError {
   public:
    BoxCalculatorError(std::shared_ptr<vrml_proc::core::error::Error> innerError) { SetInnerError(innerError); }
    BoxCalculatorError() = default;

   protected:
    std::string GetMessageInternal() const override {
      std::ostringstream stream;
      stream << CalculatorError::GetMessageInternal() << "[BoxCalculator]: error occured!\n";
      return stream.str();
    }
  };

  class IndexedFaceSetCalculatorError : public CalculatorError {
   public:
    IndexedFaceSetCalculatorError(std::shared_ptr<vrml_proc::core::error::Error> innerError) {
      SetInnerError(innerError);
    }
    IndexedFaceSetCalculatorError() = default;

   protected:
    std::string GetMessageInternal() const override {
      std::ostringstream stream;
      stream << CalculatorError::GetMessageInternal() << "[IndexedFaceSetCalculator]: error occured!\n";
      return stream.str();
    }
  };

  class IndexedLineSetCalculatorError : public CalculatorError {
   public:
    IndexedLineSetCalculatorError(std::shared_ptr<vrml_proc::core::error::Error> innerError) {
      SetInnerError(innerError);
    }
    IndexedLineSetCalculatorError() = default;

   protected:
    std::string GetMessageInternal() const override {
      std::ostringstream stream;
      stream << CalculatorError::GetMessageInternal() << "[IndexedLineSetCalculatorError]: error occured!\n";
      return stream.str();
    }
  };

  class AlphaShapeCalculatorError : public CalculatorError {
   public:
    AlphaShapeCalculatorError(std::shared_ptr<vrml_proc::core::error::Error> innerError) { SetInnerError(innerError); }
    AlphaShapeCalculatorError() = default;

   protected:
    std::string GetMessageInternal() const override {
      std::ostringstream stream;
      stream << CalculatorError::GetMessageInternal() << "[AlphaShapeCalculatorError]: error occured!\n";
      return stream.str();
    }
  };
}  // namespace to_geom::calculator::error
