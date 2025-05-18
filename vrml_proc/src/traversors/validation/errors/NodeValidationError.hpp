#pragma once

#include <sstream>
#include <string>
#include <unordered_set>

#include "Error.hpp"

namespace vrml_proc::traversor::validation::error {

  /**
   * @brief Represents generic NodeValidation error. Node validation is a process, where node is valiated against the
   * specification.
   */
  class NodeValidationError : public vrml_proc::core::error::Error {
   protected:
    virtual std::string GetMessageInternal() const { return "[NodeValidationError]"; }

    std::string ConvertStringSetToString(const std::unordered_set<std::string>& names) {
      std::ostringstream stream;
      bool isFirst = true;
      for (const auto& name : names) {
        if (!isFirst) {
          stream << ", ";
        }
        stream << "<" << name << ">";
        isFirst = false;
      }
      return stream.str();
    }
  };

  /**
   * @brief Error for node with unexpected header name.
   */
  class InvalidVrmlNodeHeader : public NodeValidationError {
   public:
    InvalidVrmlNodeHeader(const std::string& name, const std::unordered_set<std::string>& expectedNames)
        : m_name(name), m_expectedNames(ConvertStringSetToString(expectedNames)) {}

   protected:
    std::string GetMessageInternal() const override {
      std::ostringstream oss;
      oss << NodeValidationError::GetMessageInternal() << "[InvalidVrmlNodeHeader]: <" << m_name
          << "> is not expected header! Expected name are [" << m_expectedNames << "]!\n";
      return oss.str();
    }

   private:
    std::string m_name;
    std::string m_expectedNames;
  };

  /**
   * @brief Error for node with invalid field name.
   */
  class InvalidVrmlFieldName : public NodeValidationError {
   public:
    InvalidVrmlFieldName(const std::string& nodeName,
        const std::string& actualName,
        const std::unordered_set<std::string>& expectedNames)
        : m_node(nodeName), m_expectedNames(ConvertStringSetToString(expectedNames)), m_actualName(actualName) {}

   protected:
    std::string GetMessageInternal() const override {
      std::ostringstream oss;
      oss << NodeValidationError::GetMessageInternal() << "[InvalidVrmlFieldName]: VRML field <" << m_actualName
          << "> is an invalid field for VRML node <" << m_node << ">. Expected fields are: [" << m_expectedNames
          << "]!\n";
      return oss.str();
    }

   private:
    std::string m_node;
    std::string m_expectedNames;
    std::string m_actualName;
  };

  /**
   * @brief Error for node with invalid string value.
   */
  class InvalidStringValueError : public NodeValidationError {
   public:
    InvalidStringValueError(const std::string& nodeName,
        const std::string& fieldName,
        const std::string& invalidValue,
        const std::unordered_set<std::string>& expectedValues)
        : m_nodeName(nodeName),
          m_fieldName(fieldName),
          m_invalidValue(invalidValue),
          m_expectedValues(ConvertStringSetToString(expectedValues)) {}

   protected:
    std::string GetMessageInternal() const override {
      std::ostringstream oss;
      oss << NodeValidationError::GetMessageInternal() << "[InvalidStringValueError]: VRML field <" << m_fieldName
          << "> in VRML node <" << m_nodeName << "> contains invalid string value <" << m_invalidValue
          << ">. Expected values are: [" << m_expectedValues << "]!\n";
      return oss.str();
    }

   private:
    std::string m_nodeName;
    std::string m_fieldName;
    std::string m_invalidValue;
    std::string m_expectedValues;
  };

  /**
   * @brief Error for node whose field expecting VRML node contains VRML node but with invalid header name.
   */
  class InvalidVrmlNodeForGivenField : public NodeValidationError {
   public:
    InvalidVrmlNodeForGivenField(const std::string& field,
        const std::string& actualHeader,
        const std::unordered_set<std::string>& expectedHeaders)
        : m_field(field), m_expectedHeaders(ConvertStringSetToString(expectedHeaders)), m_actualHeader(actualHeader) {}

   protected:
    std::string GetMessageInternal() const override {
      std::ostringstream oss;
      oss << NodeValidationError::GetMessageInternal() << "[InvalidVrmlNodeForGivenField]: VRML node with header <"
          << m_actualHeader << "> is an invalid VRML node for field <" << m_field << ">. Expected headers are: ["
          << m_expectedHeaders << "]!\n";
      return oss.str();
    }

   private:
    std::string m_field;
    std::string m_expectedHeaders;
    std::string m_actualHeader;
  };

  /**
   * @brief Error for node, which contains duplicated field.
   */
  class DuplicatedVrmlFieldName : public NodeValidationError {
   public:
    DuplicatedVrmlFieldName(const std::string& duplicatedName) : m_duplicatedName(duplicatedName) {}

   protected:
    std::string GetMessageInternal() const override {
      std::ostringstream oss;
      oss << NodeValidationError::GetMessageInternal() << "[DuplicatedVrmlFieldName]: <" << m_duplicatedName
          << "> was found more than once inside VRML node!\n";
      return oss.str();
    }

   private:
    std::string m_duplicatedName;
  };

  /**
   * @brief Error for node, whose field contains invalid data type.
   */
  class InvalidFieldValueType : public NodeValidationError {
   public:
    InvalidFieldValueType(const std::string& fieldName, const std::string& expectedType, const std::string& actualType)
        : m_fieldName(fieldName), m_expectedType(expectedType), m_actualType(actualType) {}

   protected:
    std::string GetMessageInternal() const override {
      std::ostringstream oss;
      oss << NodeValidationError::GetMessageInternal() << "[InvalidFieldValueType]: field <" << m_fieldName
          << "> expected type <" << m_expectedType << ">, but received <" << m_actualType << ">!\n";
      return oss.str();
    }

   private:
    std::string m_fieldName;
    std::string m_expectedType;
    std::string m_actualType;
  };

  /**
   * @brief Error for USE node, whose identifier has no matching instance.
   */
  class MissingDefNodeForUseNode : public NodeValidationError {
   public:
    MissingDefNodeForUseNode(const std::string& useNodeId) : m_useNodeId(useNodeId) {}

   protected:
    std::string GetMessageInternal() const override {
      std::ostringstream oss;
      oss << NodeValidationError::GetMessageInternal() << "[MissingDefNodeForUseNode]: DEF node with id <"
          << m_useNodeId << "> was not found!\n";
      return oss.str();
    }

   private:
    std::string m_useNodeId;
  };
}  // namespace vrml_proc::traversor::validation::error
