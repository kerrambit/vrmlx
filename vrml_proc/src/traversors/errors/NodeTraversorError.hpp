#pragma once

#include <memory>
#include <sstream>
#include <string>

#include "Error.hpp"
#include "VrmlNode.hpp"

namespace vrml_proc::traversor::error {

  /**
   * @brief Error for NodeTraversor. Represents a node, which contained a validation error, or whose child contains
   * error.
   */
  class NodeTraversorError : public vrml_proc::core::error::Error {
   public:
    NodeTraversorError() : m_node(GetDefaultNode()) {}

    NodeTraversorError(
        std::shared_ptr<vrml_proc::core::error::Error> innerError, const vrml_proc::parser::VrmlNode& errorNode)
        : m_node(errorNode) {
      SetInnerError(innerError);
    }
    const vrml_proc::parser::VrmlNode& GetErrorNode() const { return m_node; }

   protected:
    std::string GetMessageInternal() const override {
      std::ostringstream oss;
      oss << "[NodeTraversorError]: when traversing node <" << m_node.header << ">, unexpected error occurred!\n";
      return oss.str();
    }

   private:
    const vrml_proc::parser::VrmlNode& m_node;

    static const vrml_proc::parser::VrmlNode& GetDefaultNode() {
      static vrml_proc::parser::VrmlNode dummyNode;
      return dummyNode;
    }
  };

  /**
   * @brief Represents an error for unknown VRML node.
   */
  class UnknownVrmlNode : public vrml_proc::core::error::Error {
   public:
    UnknownVrmlNode(std::string nodeName) : m_nodeName(nodeName) {}

   protected:
    std::string GetMessageInternal() const override {
      std::ostringstream oss;
      oss << "[UnknownVrmlNode]: found unknown VRML node with name <" << m_nodeName << ">!\n";
      return oss.str();
    }

   private:
    std::string m_nodeName;
  };
}  // namespace vrml_proc::traversor::error
