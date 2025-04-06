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

#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
  namespace parser {

    struct VrmlNode;
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
        std::vector<
            boost::variant<boost::recursive_wrapper<struct VrmlNode>, boost::recursive_wrapper<struct UseNode>>>>;
    using VrmlNodeArray = std::vector<
        boost::variant<boost::recursive_wrapper<struct VrmlNode>, boost::recursive_wrapper<struct UseNode>>>;

    struct VRMLPROCESSING_API VrmlField {
      std::string name;
      VrmlFieldValue value;

      VrmlField() {}

      VrmlField(const std::string& n, const VrmlFieldValue& v) : name(n), value(v) {}
    };
  }  // namespace parser
}  // namespace vrml_proc
