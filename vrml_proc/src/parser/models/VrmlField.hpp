#pragma once

#include <iostream>
#include <string>

#include <boost/variant.hpp>
#include <boost/variant/recursive_wrapper.hpp>

#include "Int32Array.hpp"
#include "UseNode.hpp"
#include "Vec2f.hpp"
#include "Vec2fArray.hpp"
#include "Vec3f.hpp"
#include "Vec3fArray.hpp"
#include "Vec4f.hpp"
#include "VrmlUnits.hpp"

namespace vrml_proc::parser {

  // Forward declaration.
  struct VrmlNode;

  using VrmlNodeArray =
      std::vector<boost::variant<boost::recursive_wrapper<struct VrmlNode>, boost::recursive_wrapper<struct UseNode>>>;

  using VrmlFieldValue = boost::variant<std::string,
      bool,
      Vec3fArray,
      Vec2fArray,
      Int32Array,
      float32_t,
      int32_t,
      Vec4f,
      Vec3f,
      Vec2f,
      UseNode,
      boost::recursive_wrapper<struct VrmlNode>,
      VrmlNodeArray>;

  struct VrmlField {
    VrmlField() : name(""), value(VrmlFieldValue()) {}
    VrmlField(const std::string& n, const VrmlFieldValue& v) : name(n), value(v) {}

    std::string name;
    VrmlFieldValue value;
  };
}  // namespace vrml_proc::parser
