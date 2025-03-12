#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

#include <boost/variant/recursive_wrapper.hpp>
#include <boost/variant/variant.hpp>
#include <result.hpp>

#include "NodeValidationError.hpp"
#include "TypeToString.hpp"
#include "UseNode.hpp"
#include "VrmlField.hpp"
#include "VrmlFieldExtractor.hpp"
#include "VrmlNode.hpp"
#include "VrmlNodeManager.hpp"

namespace vrml_proc {
  namespace traversor {
    namespace validator {
      /**
       * @brief Class representing a generic NodeValidator. Subclasses have to implement Validate() method.
       * The interface offers also several static helper methods.
       * Static methods work as bridge between raw outputs of VrmlFieldExtractor and nicely formatted output of
       * NodeValidationError entities.
       */
      class NodeValidator {
       public:
        /**
         * @brief Static helper method which checks that all field names are from the given list of allowed names.
         * Method also verifies that each field name is at most once and that no unknown field name is used either.
         *
         * @param validFieldNames set of all allowed field names
         * @param fields list of fields to verify
         * @returns result type where error stores the NodeValidationError subtype
         */
        static cpp::result<void, std::shared_ptr<error::NodeValidationError>> CheckForOnlyUniqueAllowedFieldNames(
            const std::unordered_set<std::string>& validFieldNames,
            const std::vector<vrml_proc::parser::VrmlField>& fields, const std::string& nodeName) {
          std::unordered_set<std::string> alreadyFoundFieldNames;
          for (const auto& field : fields) {
            if (validFieldNames.find(field.name) == validFieldNames.end()) {
              return cpp::fail(std::make_shared<error::InvalidVrmlFieldName>(nodeName, field.name, validFieldNames));
            }

            if (alreadyFoundFieldNames.find(field.name) != alreadyFoundFieldNames.end()) {
              return cpp::fail(std::make_shared<error::DuplicatedVrmlFieldName>(field.name));
            }
            alreadyFoundFieldNames.insert(field.name);
          }
          return {};
        }

        /**
         * @brief Static helper method which checks if node has valid header. Basically this method checks the "kind" of
         * generic VRML node.
         *
         * @param validHeaders set of allowed header names
         * @param node node to verify
         * @param field field which node belongs to in the parent node
         *
         * @returns result type where error stores the NodeValidationError subtype
         */
        static cpp::result<void, std::shared_ptr<error::NodeValidationError>> CheckForOnlyAllowedVrmlNodeHeaders(
            const std::unordered_set<std::string>& validHeaders, const vrml_proc::parser::VrmlNode& node,
            const std::string& field) {
          if (validHeaders.find(node.header) == validHeaders.end()) {
            return cpp::fail(std::make_shared<error::InvalidVrmlNodeForGivenField>(field, node.header, validHeaders));
          }

          return {};
        }

        /**
         * @brief Static helper method which validates field entry. Field should be of type VrmlNode (or UseNode which
         * is evaluated to VrmlNode).
         *
         * @param name name of the field
         * @param fields list of all node's fields
         * @param manager manager where associations between UseNodes and VrmlNodes will be searched for
         *
         * @returns NodeValidationError if failure occurs (invalid field type, missing VrmlNode for UseNode id);
         * otherwise it returns empty optional (field is missing) or optional containing const reference to VRML node
         */
        static cpp::result<std::optional<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>,
                           std::shared_ptr<error::NodeValidationError>>
        ExtractVrmlNodeWithValidation(const std::string& fieldName,
                                      const std::vector<vrml_proc::parser::VrmlField>& fields,
                                      const vrml_proc::parser::VrmlNodeManager& manager) {
          std::string invalidType = "";
          std::string missingUseId = "";
          auto value = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeExtended(
              fieldName, fields, manager, invalidType, missingUseId);

          if (value.has_error()) {
            if (value.error() ==
                vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeError::FieldNotFound) {
              return std::optional<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>{};
            } else if (value.error() ==
                       vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeError::ValidationError) {
              return cpp::fail(std::make_shared<error::InvalidFieldValueType>(
                  fieldName,
                  vrml_proc::core::utils::TypeToString<vrml_proc::parser::VrmlNode>() + " or " +
                      vrml_proc::core::utils::TypeToString<vrml_proc::parser::UseNode>(),
                  invalidType));
            } else if (value.error() ==
                       vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeError::UnknownUseNode) {
              return cpp::fail(std::make_shared<error::MissingDefNodeForUseNode>(missingUseId));
            }
          }

          return std::optional<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>(value.value());
        }

        /**
         * @brief Static helper method which validates field entry. Field type is templated.
         *
         * @param name name of the field
         * @param fields list of all node's fields
         *
         * @returns NodeValidationError if failure occurs (invalid field type); otherwise it returns empty optional
         * (field is missing) or optional containing const reference to T
         */
        template <typename T>
        static cpp::result<std::optional<std::reference_wrapper<const T>>, std::shared_ptr<error::NodeValidationError>>
        ExtractFieldByNameWithValidation(const std::string& fieldName,
                                         const std::vector<vrml_proc::parser::VrmlField>& fields) {
          std::string invalidType = "";
          auto value = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<T>(fieldName, fields,
                                                                                                     invalidType);

          if (value.has_error()) {
            if (value.error() ==
                vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameError::FieldNotFound) {
              return std::optional<std::reference_wrapper<const T>>{};
            } else if (value.error() ==
                       vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameError::ValidationError) {
              return cpp::fail(std::make_shared<error::InvalidFieldValueType>(
                  fieldName, vrml_proc::core::utils::TypeToString<T>(), invalidType));
            }
          }

          return std::optional<std::reference_wrapper<const T>>(value.value());
        }

        /**
         * @brief Static helper method which validates field entry of expected type VRML node array and resolves found
         * USE nodes into VRML nodes.
         *
         * @param fieldName name of the field
         * @param fields vector of node's fields
         * @param manager VRML manager
         * @param enableSingleArrayNode flag which enables to extract also signular VRML node instead of VRML node array
         *
         * @returns NodeValidationError if the field has wrong type (or if any USE node cannot found its corresponding
         * DEF node); otherwise it returns empty optional (field is missing) or optional containing vector of const
         * references to VRML nodes
         */
        static cpp::result<std::optional<std::vector<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>>,
                           std::shared_ptr<error::NodeValidationError>>
        ExtractVrmlNodeArrayWithValidation(const std::string& fieldName,
                                           const std::vector<vrml_proc::parser::VrmlField>& fields,
                                           const vrml_proc::parser::VrmlNodeManager& manager,
                                           bool enableSingleArrayNode = false) {
          if (!vrml_proc::parser::model::utils::VrmlFieldExtractor::IsNamePresent(fieldName, fields)) {
            return std::optional<std::vector<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>>{};
          }

          if (enableSingleArrayNode) {
            std::string invalidType = "";
            std::string missingUseId = "";
            auto value = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeExtended(
                fieldName, fields, manager, invalidType, missingUseId);

            if (value.has_error()) {
              if (value.error() ==
                  vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeError::UnknownUseNode) {
                return cpp::fail(std::make_shared<error::MissingDefNodeForUseNode>(missingUseId));
              }
            } else {
              std::vector<std::reference_wrapper<const vrml_proc::parser::VrmlNode>> resolvedChildren;
              resolvedChildren.push_back(value.value());
              return resolvedChildren;
            }
          }

          std::string invalidType = "";
          cpp::result<std::reference_wrapper<
                          const std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>,
                                                           boost::recursive_wrapper<vrml_proc::parser::UseNode>>>>,
                      vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameError>
              value = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<
                  std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>,
                                             boost::recursive_wrapper<vrml_proc::parser::UseNode>>>>(fieldName, fields,
                                                                                                     invalidType);

          if (value.has_error()) {
            if (value.error() ==
                vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameError::FieldNotFound) {
              return std::optional<std::vector<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>>{};
            } else if (value.error() ==
                       vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameError::ValidationError) {
              return cpp::fail(std::make_shared<error::InvalidFieldValueType>(
                  fieldName,
                  vrml_proc::core::utils::TypeToString<
                      std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>,
                                                 boost::recursive_wrapper<vrml_proc::parser::UseNode>>>>(),
                  invalidType));
            }
          }

          std::vector<std::reference_wrapper<const vrml_proc::parser::VrmlNode>> resolvedChildren;

          for (const auto& child : value.value().get()) {
            std::string invalidType;
            std::string useId;
            cpp::result<std::reference_wrapper<const vrml_proc::parser::VrmlNode>,
                        vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeFromVariantError>
                childResult = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeFromVariantExtended(
                    child, manager, invalidType, useId);
            if (childResult.has_error()) {
              switch (childResult.error()) {
                case vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeFromVariantError::
                    ValidationError:
                  return cpp::fail(std::make_shared<error::InvalidFieldValueType>(
                      fieldName, vrml_proc::core::utils::TypeToString<vrml_proc::parser::VrmlNode>(), invalidType));
                case vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeFromVariantError::
                    UnknownUseNode:
                  return cpp::fail(std::make_shared<error::MissingDefNodeForUseNode>(useId));
                default:
                  break;
              }
            }
            resolvedChildren.push_back(childResult.value());
          }

          return resolvedChildren;
        }
      };
    }  // namespace validator
  }    // namespace traversor
}  // namespace vrml_proc
