#pragma once

#include <memory>
#include <sstream>
#include <string>

#include "Error.hpp"
#include "VrmlNode.hpp"

namespace vrml_proc::traversor::error {
  /**
   * @brief Represents an error for invalid VrmlFile.
   */
  class FileTraversorError : public vrml_proc::core::error::Error {
   public:
    FileTraversorError(
        std::shared_ptr<vrml_proc::core::error::Error> innerError, const vrml_proc::parser::VrmlNode& errorNode)
        : m_node(errorNode) {
      SetInnerError(innerError);
    }
    FileTraversorError(const vrml_proc::parser::VrmlNode& errorNode) : m_node(errorNode) {}
    const vrml_proc::parser::VrmlNode& GetErrorNode() const { return m_node; }

   protected:
    std::string GetMessageInternal() const override {
      std::ostringstream stream;
      stream << "[FileTraversorError]: when traversing root <" << m_node.header << ">, unexpected error occurred!\n";
      return stream.str();
    }

   private:
    const vrml_proc::parser::VrmlNode& m_node;

    static const vrml_proc::parser::VrmlNode& GetDefaultNode() {
      static vrml_proc::parser::VrmlNode dummyNode;
      return dummyNode;
    }
  };
}  // namespace vrml_proc::traversor::error
