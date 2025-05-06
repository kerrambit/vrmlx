#include "BoxCalculator.hpp"

#include <memory>

#include <CGAL/Kernel/interface_macros.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh/Surface_mesh.h>

#include <result.hpp>

#include "CalculatorError.hpp"
#include "CalculatorResult.hpp"
#include "CGALBaseTypesForVrml.hpp"
#include "Error.hpp"
#include "FormatString.hpp"
#include "Logger.hpp"
#include "ManualTimer.hpp"
#include "Mesh.hpp"
#include "ModelValidator.hpp"

namespace to_geom {
  namespace calculator {
    to_geom::calculator::CalculatorResult BoxCalculator::Generate3DMesh(
        std::reference_wrapper<const vrml_proc::parser::Vec3f> size,
        const vrml_proc::math::TransformationMatrix& matrix) {  //

      using namespace vrml_proc::core::logger;
      using namespace vrml_proc::core::utils;
      using namespace vrml_proc::parser::model::validator;
      using vrml_proc::math::cgal::CGALPoint;

      LogDebug("Generate 3D mesh using BoxCalculator.", LOGGING_INFO);

      auto checkResult = CheckVec3fIsGreaterThanZero(size.get());
      if (checkResult.has_error()) {
        return cpp::fail(std::make_shared<error::BoxCalculatorError>()
                         << (std::make_shared<error::PropertiesError>() << checkResult.error()));
      }

      auto timer = ManualTimer();
      timer.Start();

      auto mesh = std::make_shared<to_geom::core::Mesh>();

      double half_x = size.get().x / 2.0;
      double half_y = size.get().y / 2.0;
      double half_z = size.get().z / 2.0;

      vrml_proc::math::cgal::CGALPoint vertices[8] = {/** Left back down. */
          CGALPoint(-half_x, -half_y, -half_z),
          /** Right back down. */
          CGALPoint(half_x, -half_y, -half_z),
          /** Right back up. */
          CGALPoint(half_x, half_y, -half_z),
          /** Left back up. */
          CGALPoint(-half_x, half_y, -half_z),
          /** Left front down. */
          CGALPoint(-half_x, -half_y, half_z),
          /** Right front down. */
          CGALPoint(half_x, -half_y, half_z),
          /** Right front up. */
          CGALPoint(half_x, half_y, half_z),
          /** Left front up. */
          CGALPoint(-half_x, half_y, half_z)};

      to_geom::core::Mesh::Vertex_index v[8];
      for (size_t i = 0; i < 8; ++i) {
        CGALPoint transformedPoint = matrix.transform(vertices[i]);
        v[i] = mesh->add_vertex(transformedPoint);
      }

      /** Front face. */
      mesh->add_face(v[4], v[5], v[6]);
      mesh->add_face(v[4], v[6], v[7]);

      /** Back face. */
      mesh->add_face(v[1], v[0], v[3]);
      mesh->add_face(v[1], v[3], v[2]);

      /** Top face. */
      mesh->add_face(v[7], v[6], v[2]);
      mesh->add_face(v[7], v[2], v[3]);

      /** Bottom face. */
      mesh->add_face(v[0], v[1], v[5]);
      mesh->add_face(v[0], v[5], v[4]);

      /** Right face. */
      mesh->add_face(v[5], v[1], v[2]);
      mesh->add_face(v[5], v[2], v[6]);

      /** Left face. */
      mesh->add_face(v[0], v[4], v[7]);
      mesh->add_face(v[0], v[7], v[3]);

      double time = timer.End();
      LogDebug(FormatString("Mesh was generated successfully. The generation took ", time, " seconds."), LOGGING_INFO);

      return mesh;
    }
  }  // namespace calculator
}  // namespace to_geom
