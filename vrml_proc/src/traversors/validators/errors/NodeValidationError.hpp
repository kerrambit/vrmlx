#pragma once

#include <sstream>
#include <string>
#include <unordered_set>

#include "Error.hpp"

namespace vrml_proc {
  namespace traversor {
    namespace validator {
      namespace error {

        class NodeValidationError : public vrml_proc::core::error::Error {
         protected:
          virtual std::string GetMessageInternal() const { return "[NodeValidationError]"; }
        };

        class InvalidVrmlNodeHeader : public NodeValidationError {
         public:
          InvalidVrmlNodeHeader(const std::string& name, const std::unordered_set<std::string>& expectedNames)
              : m_name(name) {
            AddExpectedNames(expectedNames);
          }

         protected:
          std::string GetMessageInternal() const override {
            std::ostringstream oss;
            oss << NodeValidationError::GetMessageInternal() << "[InvalidVrmlNodeHeader]: <" << m_name
                << "> is not expected header! Expected name are [" << m_expectedNames << "]!\n";
            return oss.str();
          }

         private:
          void AddExpectedNames(const std::unordered_set<std::string>& names) {
            std::ostringstream stream;
            bool isFirst = true;
            for (const auto& header : names) {
              if (!isFirst) {
                stream << ", ";
              }
              stream << "<" << header << ">";
              isFirst = false;
            }
            m_expectedNames = stream.str();
          }
          std::string m_name;
          std::string m_expectedNames;
        };

        class InvalidVrmlFieldName : public NodeValidationError {
         public:
          InvalidVrmlFieldName(const std::string& nodeName,
              const std::string& actualName,
              const std::unordered_set<std::string>& expectedNames)
              : m_node(nodeName), m_actualName(actualName) {
            AddExpectedNames(expectedNames);
          }

         protected:
          std::string GetMessageInternal() const override {
            std::ostringstream oss;
            oss << NodeValidationError::GetMessageInternal() << "[InvalidVrmlFieldName]: VRML field <" << m_actualName
                << "> is an invalid field for VRML node <" << m_node << ">. Expected fields are: [" << m_expectedNames
                << "]!\n";
            return oss.str();
          }

         private:
          void AddExpectedNames(const std::unordered_set<std::string>& names) {
            std::ostringstream stream;
            bool isFirst = true;
            for (const auto& header : names) {
              if (!isFirst) {
                stream << ", ";
              }
              stream << "<" << header << ">";
              isFirst = false;
            }
            m_expectedNames = stream.str();
          }
          std::string m_node;
          std::string m_expectedNames;
          std::string m_actualName;
        };

        class InvalidVrmlNodeForGivenField : public NodeValidationError {
         public:
          InvalidVrmlNodeForGivenField(const std::string& field,
              const std::string& actualHeader,
              const std::unordered_set<std::string>& expectedHeaders)
              : m_field(field), m_actualHeader(actualHeader) {
            AddExpectedHeaders(expectedHeaders);
          }

         protected:
          std::string GetMessageInternal() const override {
            std::ostringstream oss;
            oss << NodeValidationError::GetMessageInternal()
                << "[InvalidVrmlNodeForGivenField]: VRML node with header <" << m_actualHeader
                << "> is an invalid VRML node for field <" << m_field << ">. Expected headers are: ["
                << m_expectedHeaders << "]!\n";
            return oss.str();
          }

         private:
          void AddExpectedHeaders(const std::unordered_set<std::string>& headers) {
            std::ostringstream stream;
            bool isFirst = true;
            for (const auto& header : headers) {
              if (!isFirst) {
                stream << ", ";
              }
              stream << "<" << header << ">";
              isFirst = false;
            }
            m_expectedHeaders = stream.str();
          }
          std::string m_field;
          std::string m_expectedHeaders;
          std::string m_actualHeader;
        };

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

        class InvalidFieldValueType : public NodeValidationError {
         public:
          InvalidFieldValueType(
              const std::string& fieldName, const std::string& expectedType, const std::string& actualType)
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

        class MissingDefNodeForUseNode : public NodeValidationError {
         public:
          MissingDefNodeForUseNode(const std::string& useNodeId) : m_useNodeId(useNodeId) {}

         protected:
          std::string GetMessageInternal() const override {
            std::ostringstream oss;
            oss << NodeValidationError::GetMessageInternal() << "[MissingDefNodeForUseNode]: USE node with id <"
                << m_useNodeId << "> was not found!\n";
            return oss.str();
          }

         private:
          std::string m_useNodeId;
        };
      }  // namespace error
    }    // namespace validator
  }      // namespace traversor
}  // namespace vrml_proc
