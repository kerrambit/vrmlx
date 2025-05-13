#pragma once

#include <string>
#include <vector>

#include <boost/optional.hpp>

#include "VrmlField.hpp"

namespace vrml_proc::parser::model {
  /**
   * @brief Represents a VRML node.
   */
  struct VrmlNode {
    VrmlNode() : definitionName(""), header(""), fields(std::vector<VrmlField>()) {}

    boost::optional<std::string> definitionName;
    std::string header;
    std::vector<VrmlField> fields;
  };
}  // namespace vrml_proc::parser::model
