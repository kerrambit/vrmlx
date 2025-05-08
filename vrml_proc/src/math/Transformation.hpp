#pragma once

#include "Vec3f.hpp"
#include "Vec4f.hpp"

namespace vrml_proc::math {

  /**
   * @brief Represents a Transformation object, which is used in VRML 2.0 format. It basically mirros the data inside
   * Transform node.
   */
  struct Transformation {
    vrml_proc::parser::Vec3f center;
    vrml_proc::parser::Vec4f rotation;
    vrml_proc::parser::Vec3f scale;
    vrml_proc::parser::Vec4f scaleOrientation;
    vrml_proc::parser::Vec3f translation;

    Transformation()
        : center(vrml_proc::parser::Vec3f()),
          rotation(vrml_proc::parser::Vec4f(0.0f, 0.0f, 1.0f, 0.0f)),
          scale(vrml_proc::parser::Vec3f(1.0f, 1.0f, 1.0f)),
          scaleOrientation(vrml_proc::parser::Vec4f(0.0f, 0.0f, 1.0f, 0.0f)),
          translation(vrml_proc::parser::Vec3f()) {}
  };
}  // namespace vrml_proc::math
