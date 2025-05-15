#pragma once

#include "CalculatorError.hpp"
#include "CalculatorResult.hpp"
#include "ModelValidationError.hpp"
#include "Range.hpp"

namespace to_geom::calculator::CalculatorUtils {

  template <typename CalculatorErrorType>
  inline to_geom::calculator::CalculatorResult ReturnVertexIndexOutOfRangeError(
      const vrml_proc::core::utils::Range<int32_t>& range, int32_t actualValue) {  //

    using namespace to_geom::calculator::error;
    using vrml_proc::parser::model::validator::error::NumberOutOfRangeError;

    return cpp::fail(std::make_shared<CalculatorErrorType>()
                     << (std::make_shared<PropertiesError>()
                            << (std::make_shared<VertexIndexOutOfRangeError>()
                                   << std::make_shared<NumberOutOfRangeError<int32_t>>(range, actualValue))));
  }
}  // namespace to_geom::calculator::CalculatorUtils
