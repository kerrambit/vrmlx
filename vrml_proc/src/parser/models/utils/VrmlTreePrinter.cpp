#include "VrmlTreePrinter.hpp"

#include <ranges>

using namespace vrml_proc::parser::model::utils;

/**
 * @brief Helper visitor which prints given obejcts from VrmlField using VrmlTreePrinter.
 */
struct Visitor {
  std::ostream* outputStream;
  VrmlTreePrinter::IndentationLevel indentationLevel;
  VrmlTreePrinter printer;

  Visitor(std::ostream* outputStream, VrmlTreePrinter::IndentationLevel indentationLevel)
      : outputStream(outputStream), indentationLevel(indentationLevel), printer(*outputStream) {}

  void operator()(const std::string& string) const {
    *outputStream << VrmlTreePrinter::CreateIndentationString(indentationLevel) << "String (" << &string << "): <"
                  << string << ">";
    *outputStream << std::endl;
  }

  void operator()(const bool& boolean) const {
    *outputStream << VrmlTreePrinter::CreateIndentationString(indentationLevel) << "Boolean (" << &boolean << "): <"
                  << std::boolalpha << boolean << ">";
    *outputStream << std::endl;
  }

  void operator()(const vrml_proc::parser::model::Vec2fArray& array) const { printer.Print(array, indentationLevel); }

  void operator()(const vrml_proc::parser::model::Vec3fArray& array) const { printer.Print(array, indentationLevel); }

  void operator()(const vrml_proc::parser::model::Int32Array& array) const { printer.Print(array, indentationLevel); }

  void operator()(vrml_proc::parser::model::float32_t value) const {
    *outputStream << VrmlTreePrinter::CreateIndentationString(indentationLevel) << "Float32 (" << &value << "): <"
                  << value << ">";
    *outputStream << std::endl;
  }

  void operator()(int32_t value) const {
    *outputStream << VrmlTreePrinter::CreateIndentationString(indentationLevel) << "Int32 (" << &value << "): <"
                  << value << ">";
    *outputStream << std::endl;
  }

  void operator()(const vrml_proc::parser::model::Vec2f& vector) const { printer.Print(vector, indentationLevel); }

  void operator()(const vrml_proc::parser::model::Vec3f& vector) const { printer.Print(vector, indentationLevel); }

  void operator()(const vrml_proc::parser::model::Vec4f& vector) const { printer.Print(vector, indentationLevel); }

  void operator()(const vrml_proc::parser::model::UseNode& node) const { printer.Print(node, indentationLevel); }

  void operator()(const boost::recursive_wrapper<vrml_proc::parser::model::VrmlNode>& node) const {
    printer.Print(node.get(), indentationLevel);
  }

  void operator()(const std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::model::VrmlNode>,
          boost::recursive_wrapper<vrml_proc::parser::model::UseNode>>>& nodes) const {  //

    *outputStream << VrmlTreePrinter::CreateIndentationString(indentationLevel) << "VRMLNodeArray (" << &nodes
                  << "):\n";
    *outputStream << VrmlTreePrinter::CreateIndentationString(indentationLevel + 1) << "[\n";

    struct PrintVisitor {
      std::ostream* outputStream;
      VrmlTreePrinter::IndentationLevel indentationLevel;
      VrmlTreePrinter printer;

      PrintVisitor(std::ostream* outputStream, VrmlTreePrinter::IndentationLevel indentationLevel)
          : outputStream(outputStream), indentationLevel(indentationLevel), printer(*outputStream) {}

      void operator()(const boost::recursive_wrapper<vrml_proc::parser::model::VrmlNode>& node) const {
        printer.Print(node.get(), indentationLevel);
      }

      void operator()(const boost::recursive_wrapper<vrml_proc::parser::model::UseNode>& node) const {
        printer.Print(node.get(), indentationLevel);
      }
    };

    for (const auto& node : nodes) {
      boost::apply_visitor(PrintVisitor(outputStream, indentationLevel + 2), node);
    }

    *outputStream << VrmlTreePrinter::CreateIndentationString(indentationLevel + 1) << "]";
  }
};

// ----------------------------------------------------------------------------------------------------------//

void VrmlTreePrinter::Print(const vrml_proc::parser::model::VrmlNode& object, IndentationLevel indentationLevel) const {
  std::string indentationString = VrmlTreePrinter::CreateIndentationString(indentationLevel);
  indentationLevel++;

  *m_stream << indentationString;
  *m_stream << "VRMLNode (" << &object << "):\n";
  if (object.definitionName.has_value() && object.definitionName.value() != "") {
    *m_stream << VrmlTreePrinter::CreateIndentationString(indentationLevel) << "DEF identifier: <"
              << object.definitionName.value() << ">\n";
  }
  *m_stream << VrmlTreePrinter::CreateIndentationString(indentationLevel) << "Header: <" << object.header << ">\n";
  *m_stream << VrmlTreePrinter::CreateIndentationString(indentationLevel) << "Fields:\n";

  for (const auto& field : object.fields) {
    Print(field, indentationLevel + 1);
  }
}

void VrmlTreePrinter::Print(const vrml_proc::parser::model::UseNode& object, IndentationLevel indentationLevel) const {
  std::string indentationString = VrmlTreePrinter::CreateIndentationString(indentationLevel);
  indentationLevel++;

  *m_stream << indentationString;
  *m_stream << "USENode (" << &object << "):\n";
  *m_stream << VrmlTreePrinter::CreateIndentationString(indentationLevel) << "Identifier: <" << object.identifier
            << ">";
  *m_stream << std::endl;
}

void VrmlTreePrinter::Print(const vrml_proc::parser::model::Int32Array& object, IndentationLevel indentationLevel) const {  //

  std::string indentationString = VrmlTreePrinter::CreateIndentationString(indentationLevel);
  indentationLevel++;

  *m_stream << indentationString;
  *m_stream << "Int32Array (" << &object << "):\n";
  *m_stream << VrmlTreePrinter::CreateIndentationString(indentationLevel) << "[\n"
            << VrmlTreePrinter::CreateIndentationString(indentationLevel + 1);

  for (const auto& entity : object.integers | std::views::take(3)) {
    *m_stream << "<" << entity << "> ";
  }

  if (object.integers.size() > 3) {
    *m_stream << "(...other " << object.integers.size() - 3 << " elements) ]";
  }

  *m_stream << "\n";
  *m_stream << std::endl;
}

void VrmlTreePrinter::Print(const vrml_proc::parser::model::Vec2f& object, IndentationLevel indentationLevel) const {
  std::string indentationString = VrmlTreePrinter::CreateIndentationString(indentationLevel);
  indentationLevel++;

  *m_stream << indentationString;
  *m_stream << "Vec2f (" << &object << "):\n";
  *m_stream << VrmlTreePrinter::CreateIndentationString(indentationLevel) << "( <" << object.u << "> <" << object.v
            << "> )" << std::endl;
}

void VrmlTreePrinter::Print(const vrml_proc::parser::model::Vec2fArray& object, IndentationLevel indentationLevel) const {
  std::string indentationString = VrmlTreePrinter::CreateIndentationString(indentationLevel);
  indentationLevel++;

  *m_stream << indentationString;
  *m_stream << "Vec2fArray (" << &object << "):\n";
  *m_stream << VrmlTreePrinter::CreateIndentationString(indentationLevel) << "[\n";

  for (const auto& entity : object.vectors | std::views::take(3)) {
    Print(entity, indentationLevel + 1);
  }

  if (object.vectors.size() > 3) {
    *m_stream << "\n"
              << VrmlTreePrinter::CreateIndentationString(indentationLevel + 1) << " (...other "
              << object.vectors.size() - 3 << " elements) ]";
  }

  *m_stream << std::endl;
}

void VrmlTreePrinter::Print(const vrml_proc::parser::model::Vec3f& object, IndentationLevel indentationLevel) const {
  std::string indentationString = VrmlTreePrinter::CreateIndentationString(indentationLevel);
  indentationLevel++;

  *m_stream << indentationString;
  *m_stream << "Vec3f (" << &object << "):\n";
  *m_stream << VrmlTreePrinter::CreateIndentationString(indentationLevel) << "( <" << object.x << "> <" << object.y
            << "> <" << object.z << "> )" << std::endl;
}

void VrmlTreePrinter::Print(const vrml_proc::parser::model::Vec3fArray& object, IndentationLevel indentationLevel) const {
  std::string indentationString = VrmlTreePrinter::CreateIndentationString(indentationLevel);
  indentationLevel++;

  *m_stream << indentationString;
  *m_stream << "Vec3fArray (" << &object << "):\n";
  *m_stream << VrmlTreePrinter::CreateIndentationString(indentationLevel) << "[\n";

  for (const auto& entity : object.vectors | std::views::take(3)) {
    Print(entity, indentationLevel + 1);
  }

  if (object.vectors.size() > 3) {
    *m_stream << "\n"
              << VrmlTreePrinter::CreateIndentationString(indentationLevel + 1) << " (...other "
              << object.vectors.size() - 3 << " elements) ]";
  }

  *m_stream << std::endl;
}

void VrmlTreePrinter::Print(const vrml_proc::parser::model::Vec4f& object, IndentationLevel indentationLevel) const {
  std::string indentationString = VrmlTreePrinter::CreateIndentationString(indentationLevel);
  indentationLevel++;

  *m_stream << indentationString;
  *m_stream << "Vec4f (" << &object << "):\n";
  *m_stream << VrmlTreePrinter::CreateIndentationString(indentationLevel) << "( <" << object.x << "> <" << object.y
            << "> <" << object.z << "> <" << object.w << "> )" << std::endl;
}

void VrmlTreePrinter::Print(const vrml_proc::parser::model::VrmlField& object, IndentationLevel indentationLevel) const {  //

  std::string indentation_string = VrmlTreePrinter::CreateIndentationString(indentationLevel);
  *m_stream << indentation_string;
  *m_stream << "VRMLField (" << &object << "):\n";
  *m_stream << VrmlTreePrinter::CreateIndentationString(indentationLevel + 1) << "Name: <" << object.name << ">\n";
  *m_stream << VrmlTreePrinter::CreateIndentationString(indentationLevel + 1) << "Value:\n";
  boost::apply_visitor(Visitor(m_stream, indentationLevel + 2), object.value);
}
