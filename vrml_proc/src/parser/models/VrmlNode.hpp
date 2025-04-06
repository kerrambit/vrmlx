#pragma once

#include <string>
#include <vector>
#include <iostream>

#include <boost/optional.hpp>

#include "VrmlField.hpp"

#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
  namespace parser {
    struct VRMLPROCESSING_API VrmlNode {
      VrmlNode() : definitionName(""), header(""), fields(std::vector<VrmlField>()) {}

      boost::optional<std::string> definitionName;
      std::string header;
      std::vector<VrmlField> fields;
    };
  }  // namespace parser
}  // namespace vrml_proc
