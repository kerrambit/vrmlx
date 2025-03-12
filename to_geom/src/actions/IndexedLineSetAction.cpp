#include "IndexedLineSetAction.hpp"

#include <functional>
#include <memory>

#include <result.hpp>

#include "ConversionContextAction.hpp"
#include "ConversionContextActionMap.hpp"
#include "GeometryAction.hpp"
#include "IndexedLineSetCalculator.hpp"
#include "Logger.hpp"
#include "MeshTaskConversionContext.hpp"
#include "TransformationMatrix.hpp"
#include "Vec3fArray.hpp"
#include "Vec3fArrayConversionContext.hpp"
#include "VrmlNodeManager.hpp"
#include "VrmlNodeTraversor.hpp"

class HelperCoordAction
    : public vrml_proc::action::ConversionContextAction<to_geom::conversion_context::Vec3fArrayConversionContext> {
 public:
  HelperCoordAction(std::reference_wrapper<const vrml_proc::parser::Vec3fArray> data) : m_data(data) {}

  std::shared_ptr<to_geom::conversion_context::Vec3fArrayConversionContext> Execute() {
    auto toReturn = std::make_shared<to_geom::conversion_context::Vec3fArrayConversionContext>();
    toReturn->CopyAndAdd(m_data);
    return toReturn;
  }

 private:
  std::reference_wrapper<const vrml_proc::parser::Vec3fArray> m_data;
};

namespace to_geom::action {

  IndexedLineSetAction::IndexedLineSetAction(IndexedLineSetAction::Properties properties,
                                             GeometryAction::Properties geometryProperties)
      : to_geom::action::GeometryAction(geometryProperties), m_properties(properties) {}

  std::shared_ptr<to_geom::conversion_context::MeshTaskConversionContext> IndexedLineSetAction::Execute() {
    vrml_proc::core::logger::LogInfo("Execute IndexedLineSetAction.", LOGGING_INFO);

    auto result = std::make_shared<to_geom::conversion_context::MeshTaskConversionContext>();

    if (!m_geometryProperties.containedByShape) {
      vrml_proc::core::logger::LogDebug(
          "Return empty data because IndexedLineSetAction node is not a child of a Shape node.", LOGGING_INFO);
      return result;
    }

    // ------------------------------------------------------------------- //

    vrml_proc::parser::VrmlNodeManager manager;
    vrml_proc::action::ConversionContextActionMap<conversion_context::Vec3fArrayConversionContext> map;

    map.AddAction("Coordinate", [this](const auto& refArgs, const auto& copyArgs) {
      if (refArgs.size() == 1 &&
          refArgs[0].get().type() == typeid(std::reference_wrapper<const vrml_proc::parser::Vec3fArray>)) {
        auto coord = std::any_cast<std::reference_wrapper<const vrml_proc::parser::Vec3fArray>>(refArgs[0]);
        return std::make_shared<HelperCoordAction>(coord);
      }
      vrml_proc::core::logger::LogFatal("Invalid arguments for CoordinateAction!", LOGGING_INFO);
      throw std::runtime_error("Invalid arguments for CoordinateAction");
    });

    vrml_proc::core::config::VrmlProcConfig config;
    auto coordResult =
        vrml_proc::traversor::VrmlNodeTraversor::Traverse<conversion_context::Vec3fArrayConversionContext>(
            {m_properties.coord.get(), manager, false, vrml_proc::math::TransformationMatrix(), config}, map);
    std::shared_ptr<to_geom::conversion_context::Vec3fArrayConversionContext> coord;
    if (coordResult.has_value()) {
      coord = coordResult.value();
    }

    if (coordResult.value()->GetData().empty()) {
      vrml_proc::core::logger::LogDebug("Return empty data because IndexedLineSet node has no points.", LOGGING_INFO);
      return result;
    }

    std::reference_wrapper<const vrml_proc::parser::Vec3fArray> points =
        std::cref((coordResult.value())->GetData().at(0));
    std::reference_wrapper<const vrml_proc::parser::Int32Array> indices = m_properties.coordIndex;
    vrml_proc::math::TransformationMatrix matrix = m_geometryProperties.matrix;

    result->Add([=]() {
      to_geom::calculator::IndexedLineSetCalculator calculator = to_geom::calculator::IndexedLineSetCalculator();
      return calculator.Generate3DMesh(indices, points, matrix);
    });

    return result;
  }
}  // namespace to_geom::action
