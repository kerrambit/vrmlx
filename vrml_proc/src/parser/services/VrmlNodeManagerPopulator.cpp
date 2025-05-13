#include "VrmlNodeManagerPopulator.hpp"

#include "VrmlFieldExtractor.hpp"
#include "VrmlNode.hpp"
#include "VrmlNodeManager.hpp"

namespace vrml_proc::parser::service::VrmlNodeManagerPopulator {
  void Populate(VrmlNodeManager& manager, const VrmlNode& node) {  //

    using namespace model::utils::VrmlFieldExtractor;

    if (node.definitionName.has_value() && node.definitionName.value() != "") {
      manager.AddDefinitionNode(node.definitionName.value(), node);
    }

    if (node.fields.size() == 0) {
      return;
    }

    for (const auto& child : node.fields) {
      auto nodeResult = Extract<VrmlNode>(child.value);
      if (nodeResult.has_value()) {
        Populate(manager, nodeResult.value().get());
        continue;
      }

      auto arrayResult = Extract<VrmlNodeArray>(child.value);
      if (arrayResult.has_value() && arrayResult.value().get().size() != 0) {
        for (const auto& variant : arrayResult.value().get()) {
          auto variantResult = ExtractVrmlNodeFromVariantWithoutResolving<VrmlNode>(variant);
          if (variantResult.has_value()) {
            Populate(manager, variantResult.value().get());
          }
        }
      }
    }
  }
}  // namespace vrml_proc::parser::service::VrmlNodeManagerPopulator
