#pragma once

#include <functional>
#include <memory>

#include "ConversionContextAction.hpp"
#include "Vec3fArray.hpp"
#include "Vec3fArrayConversionContext.hpp"

namespace to_geom::action {
  /**
   * @brief Represents an action for Coordinate VRML node.
   *
   * Inherits from ConversionContextAction<Vec3fArrayConversionContext>.
   */
  class HelperCoordinateAction
      : public vrml_proc::action::ConversionContextAction<to_geom::conversion_context::Vec3fArrayConversionContext> {
   public:
    /**
     * @brief Properties for `HelperCoordinateAction`.
     */
    struct Properties {
      /** @brief Points inside Coordinate VRML node. */
      std::reference_wrapper<const vrml_proc::parser::Vec3fArray> points;
    };

    /**
     * @brief Constructs a HelperCoordinateAction with specified properties.
     * @param properties properties for HelperCoordinateAction
     */
    HelperCoordinateAction(Properties properties) : m_properties(properties) {}

    /**
     * @brief Takes all points by a reference and add them to Vec3fArrayConversionContext.
     * @return A shared pointer to the Vec3fArrayConversionContext.
     */
    std::shared_ptr<to_geom::conversion_context::Vec3fArrayConversionContext> Execute() {
      auto result = std::make_shared<to_geom::conversion_context::Vec3fArrayConversionContext>();
      result->CopyAndAdd(m_properties.points);
      return result;
    }

   private:
    Properties m_properties;
  };
}  // namespace to_geom::action
