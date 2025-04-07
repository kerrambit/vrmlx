#include "IndexedLineSetAction.hpp"

#include <functional>
#include <memory>

#include <result.hpp>

#include "ConversionContextAction.hpp"
#include "ConversionContextActionMap.hpp"
#include "GeometryAction.hpp"
#include "HelperCoordinateAction.hpp"
#include "IndexedLineSetCalculator.hpp"
#include "Int32Array.hpp"
#include "Logger.hpp"
#include "MeshTaskConversionContext.hpp"
#include "TransformationMatrix.hpp"
#include "Vec3fArray.hpp"
#include "Vec3fArrayConversionContext.hpp"
#include "VrmlNodeManager.hpp"
#include "VrmlNodeTraversor.hpp"
#include "VrmlProcConfig.hpp"
#include "NodeView.hpp"
#include "HandlerToActionBundle.hpp"

namespace to_geom::action {

  IndexedLineSetAction::IndexedLineSetAction(
      IndexedLineSetAction::Properties properties, GeometryAction::Properties geometryProperties)
      : to_geom::action::GeometryAction(geometryProperties), m_properties(properties) {}

  std::shared_ptr<to_geom::conversion_context::MeshTaskConversionContext> IndexedLineSetAction::Execute() {
    using conversion_context::Vec3fArrayConversionContext;
    using to_geom::conversion_context::MeshTaskConversionContext;
    using vrml_proc::core::config::VrmlProcConfig;
    using vrml_proc::math::TransformationMatrix;
    using vrml_proc::parser::Int32Array;
    using vrml_proc::parser::Vec3fArray;
    using vrml_proc::traversor::VrmlNodeTraversor::Traverse;
    using namespace vrml_proc::core::logger;

    LogInfo("Execute IndexedLineSetAction.", LOGGING_INFO);

    auto result = std::make_shared<MeshTaskConversionContext>();

    if (!m_geometryProperties.containedByShape) {
      LogDebug("Return empty data because IndexedLineSetAction node is not a child of a Shape node.", LOGGING_INFO);
      return result;
    }

    /**
     * It is necessary to traverse Coordinate VRML node, which is geometry primitive node (and thus has not been
     * traversed yet). We need to construct a manager and CC map for it and then traverse it. Result is of type
     * Vec3fArrayConversionContext, which stores a reference to data we need. Note that this geometry primitive node
     * should have been validated already in the IndexedLineSet handler.
     */
    vrml_proc::parser::VrmlNodeManager manager;
    vrml_proc::action::ConversionContextActionMap<Vec3fArrayConversionContext> map;

    map.AddActionForHandlerToActionBundle(
        "Coordinate", [this](vrml_proc::traversor::handler::HandlerToActionBundle<Vec3fArrayConversionContext> data) {
          try {
            return std::make_shared<HelperCoordinateAction>(HelperCoordinateAction::Properties{
                data.nodeView->GetField<std::reference_wrapper<const vrml_proc::parser::Vec3fArray>>("point")});
          } catch (const std::bad_any_cast& e) {
            LogFatal("Invalid arguments for HelperCoordinateAction!", LOGGING_INFO);
            throw;
          }
        });

    auto coordResult = Traverse<Vec3fArrayConversionContext>(
        {m_properties.coord.get(), manager, false, TransformationMatrix(), VrmlProcConfig()}, map);

    /**
     * Geometry primitive node has been traversed, so we can check if it has any data.
     */
    if (coordResult.has_error()) {
      LogError(
          "Unexpectedly, when traversing geometry primitive node, an error orrcured. Please, check if this node has "
          "been correctly validated in a given handler. Empty data will be returned!",
          LOGGING_INFO);
      return result;
    }

    if (coordResult.value()->GetData().empty()) {
      LogDebug("Return empty data because IndexedLineSet node has no points.", LOGGING_INFO);
      return result;
    }

    std::reference_wrapper<const Vec3fArray> points = std::cref((coordResult.value())->GetData().at(0));
    std::reference_wrapper<const Int32Array> indices = m_properties.coordIndex;
    TransformationMatrix matrix = m_geometryProperties.matrix;

    result->Add([=]() {
      calculator::IndexedLineSetCalculator calculator = calculator::IndexedLineSetCalculator();
      return calculator.Generate3DMesh(indices, points, matrix);
    });

    return result;
  }
}  // namespace to_geom::action
