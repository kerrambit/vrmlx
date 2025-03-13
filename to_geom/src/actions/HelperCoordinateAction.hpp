#pragma once

#include <functional>
#include <memory>

#include "ConversionContextAction.hpp"
#include "Vec3fArray.hpp"
#include "Vec3fArrayConversionContext.hpp"

namespace to_geom::action {

  class HelperCoordinateAction
      : public vrml_proc::action::ConversionContextAction<to_geom::conversion_context::Vec3fArrayConversionContext> {
   public:
    HelperCoordinateAction(std::reference_wrapper<const vrml_proc::parser::Vec3fArray> data) : m_data(data) {}

    std::shared_ptr<to_geom::conversion_context::Vec3fArrayConversionContext> Execute() {
      auto toReturn = std::make_shared<to_geom::conversion_context::Vec3fArrayConversionContext>();
      toReturn->CopyAndAdd(m_data);
      return toReturn;
    }

   private:
    std::reference_wrapper<const vrml_proc::parser::Vec3fArray> m_data;
  };
}  // namespace to_geom::action
