#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

#include "VrmlNode.hpp"
#include "VrmlProcExport.hpp"

namespace vrml_proc::parser::service {
  /**
   * @brief Manages DEF-defined VRML nodes, enabling lookup and registration.
   *
   * This class stores references to VRML nodes defined using the `DEF` keyword,
   * allowing later retrieval through `USE` references. It is used to maintain a
   * mapping between node identifiers and their corresponding nodes without copying them.
   */
  class VRMLPROC_API VrmlNodeManager {
   public:
    /**
     * @brief Constructs an empty VRML node manager.
     */
    VrmlNodeManager();

    /**
     * @brief Adds a DEF-defined node under the specified identifier.
     *
     * @param id identifier under which to store the node
     * @param node node to be associated with the identifier
     */
    void AddDefinitionNode(const std::string& id, const VrmlNode& node);

    /**
     * @brief Retrieves a node associated with the given identifier.
     *
     * @param id identifier to look up
     * @return const reference to the node if found; std::nullopt otherwise
     */
    std::optional<std::reference_wrapper<const VrmlNode>> GetDefinitionNode(const std::string& id) const;

    /**
     * @brief returns the total number of registered DEF nodes
     * @returns total number of registered DEF nodes
     */
    inline size_t GetDefNodesTotalCount() const { return m_definitionNodes.size(); }

   private:
    std::unordered_map<std::string, std::reference_wrapper<const VrmlNode>> m_definitionNodes;
  };
}  // namespace vrml_proc::parser::service
