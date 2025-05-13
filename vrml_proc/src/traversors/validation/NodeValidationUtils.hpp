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

namespace vrml_proc::traversor::validation {
  /**
   * @brief Groups static helper functions to work with VRML node more easily.
   * Functions work as a bridge between raw outputs of VrmlFieldExtractor and nicely formatted output of
   * NodeValidationError entities. Validate method encapsulates the underlying methods and logic.
   */
  namespace NodeValidationUtils {
    /**
     * @brief Static helper method which checks that all field names are from the given list of allowed names.
     * Method also verifies that each field name is used at most once and that no unknown field name is used either.
     *
     * @param validFieldNames set of all allowed field names
     * @param fields list of fields to verify
     * @returns result type where error stores the NodeValidationError subtype
     */
    inline cpp::result<void, std::shared_ptr<error::NodeValidationError>> CheckForOnlyUniqueAllowedFieldNames(
        const std::unordered_set<std::string>& validFieldNames,
        const std::vector<vrml_proc::parser::VrmlField>& fields,
        const std::string& nodeName) {  //

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
     * @brief Static helper method which checks if node has valid header. Basically this method checks the "kind" of a
     * VRML node.
     *
     * @param validHeaders set of allowed header names, if set is empty, no check will be run
     * @param node node to verify
     * @param field field which node belongs to in the node
     *
     * @returns result type where error stores the NodeValidationError subtype
     */
    inline cpp::result<void, std::shared_ptr<error::NodeValidationError>> CheckForOnlyAllowedVrmlNodeHeaders(
        const std::unordered_set<std::string>& validHeaders,
        const vrml_proc::parser::VrmlNode& node,
        const std::string& field) {  //

      if (validHeaders.empty()) {
        return {};
      }

      if (validHeaders.find(node.header) == validHeaders.end()) {
        return cpp::fail(std::make_shared<error::InvalidVrmlNodeForGivenField>(field, node.header, validHeaders));
      }

      return {};
    }

    /**
     * @brief Static helper method which validates field entry of type VrmlNode (or UseNode which
     * is resolved to VrmlNode).
     *
     * @param name name of the expected field
     * @param fields list of all node's fields
     * @param manager manager where associations between UseNodes and VrmlNodes will be searched for
     *
     * @returns NodeValidationError if failure occurs (invalid field type, missing VrmlNode for UseNode id);
     * otherwise it returns empty optional (field is missing) or const reference to VRML node
     */
    inline cpp::result<std::optional<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>,
        std::shared_ptr<error::NodeValidationError>>
    ExtractVrmlNodeWithValidation(const std::string& fieldName,
        const std::vector<vrml_proc::parser::VrmlField>& fields,
        const vrml_proc::parser::service::VrmlNodeManager& manager) {  //

      using namespace vrml_proc::parser::model::utils::VrmlFieldExtractor;
      using namespace vrml_proc::parser;
      using vrml_proc::core::utils::TypeToString;

      std::string invalidType = "";
      std::string missingUseId = "";
      auto value = ExtractVrmlNodeExtended(fieldName, fields, manager, invalidType, missingUseId);

      if (value.has_error()) {
        if (value.error() == ExtractVrmlNodeError::FieldNotFound) {
          return std::optional<std::reference_wrapper<const VrmlNode>>{};
        } else if (value.error() == ExtractVrmlNodeError::ValidationError) {
          return cpp::fail(std::make_shared<error::InvalidFieldValueType>(
              fieldName, TypeToString<VrmlNode>() + " or " + TypeToString<UseNode>(), invalidType));
        } else if (value.error() == ExtractVrmlNodeError::UnknownUseNode) {
          return cpp::fail(std::make_shared<error::MissingDefNodeForUseNode>(missingUseId));
        }
      }

      return std::optional<std::reference_wrapper<const VrmlNode>>(value.value());
    }

    /**
     * @brief Static helper method which validates field entry. Field type is templated.
     *
     * @tparam
     * @param name name of the field
     * @param fields list of all node's fields
     *
     * @returns NodeValidationError if failure occurs (invalid field type); otherwise it returns empty optional
     * (field is missing) or optional containing const reference to T
     */
    template <typename ExpectedType>
    inline cpp::result<std::optional<std::reference_wrapper<const ExpectedType>>,
        std::shared_ptr<error::NodeValidationError>>
    ExtractFieldByNameWithValidation(
        const std::string& fieldName, const std::vector<vrml_proc::parser::VrmlField>& fields) {  //

      using namespace vrml_proc::parser::model::utils::VrmlFieldExtractor;
      using namespace vrml_proc::parser;
      using vrml_proc::core::utils::TypeToString;

      std::string invalidType = "";
      auto value = ExtractByNameExtended<ExpectedType>(fieldName, fields, invalidType);

      if (value.has_error()) {
        if (value.error() == ExtractByNameError::FieldNotFound) {
          return std::optional<std::reference_wrapper<const ExpectedType>>{};
        } else if (value.error() == ExtractByNameError::ValidationError) {
          return cpp::fail(
              std::make_shared<error::InvalidFieldValueType>(fieldName, TypeToString<ExpectedType>(), invalidType));
        }
      }

      return std::optional<std::reference_wrapper<const ExpectedType>>(value.value());
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
    inline cpp::result<std::optional<std::vector<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>>,
        std::shared_ptr<error::NodeValidationError>>
    ExtractVrmlNodeArrayWithValidation(const std::string& fieldName,
        const std::vector<vrml_proc::parser::VrmlField>& fields,
        const vrml_proc::parser::service::VrmlNodeManager& manager,
        bool enableSingleArrayNode = false) {  //

      using namespace vrml_proc::parser::model::utils::VrmlFieldExtractor;
      using namespace vrml_proc::parser;
      using vrml_proc::core::utils::TypeToString;

      if (!IsNamePresent(fieldName, fields)) {
        return std::optional<std::vector<std::reference_wrapper<const VrmlNode>>>{};
      }

      if (enableSingleArrayNode) {
        std::string invalidType = "";
        std::string missingUseId = "";
        auto value = ExtractVrmlNodeExtended(fieldName, fields, manager, invalidType, missingUseId);

        if (value.has_error()) {
          if (value.error() == ExtractVrmlNodeError::UnknownUseNode) {
            return cpp::fail(std::make_shared<error::MissingDefNodeForUseNode>(missingUseId));
          }
        } else {
          std::vector<std::reference_wrapper<const VrmlNode>> resolvedChildren;
          resolvedChildren.push_back(value.value());
          return resolvedChildren;
        }
      }

      std::string invalidType = "";
      auto value = ExtractByNameExtended<VrmlNodeArray>(fieldName, fields, invalidType);

      if (value.has_error()) {
        if (value.error() == ExtractByNameError::FieldNotFound) {
          return std::optional<std::vector<std::reference_wrapper<const VrmlNode>>>{};
        } else if (value.error() == ExtractByNameError::ValidationError) {
          return cpp::fail(
              std::make_shared<error::InvalidFieldValueType>(fieldName, TypeToString<VrmlNodeArray>(), invalidType));
        }
      }

      std::vector<std::reference_wrapper<const VrmlNode>> resolvedChildren;

      for (const auto& child : value.value().get()) {
        std::string invalidType;
        std::string useId;
        auto childResult = ExtractVrmlNodeFromVariantExtended(child, manager, invalidType, useId);
        if (childResult.has_error()) {
          switch (childResult.error()) {
            case ExtractVrmlNodeFromVariantError::ValidationError:
              return cpp::fail(std::make_shared<error::InvalidFieldValueType>(
                  fieldName, TypeToString<vrml_proc::parser::VrmlNode>(), invalidType));
            case ExtractVrmlNodeFromVariantError::UnknownUseNode:
              return cpp::fail(std::make_shared<error::MissingDefNodeForUseNode>(useId));
            default:
              break;
          }
        }
        resolvedChildren.push_back(childResult.value());
      }

      return resolvedChildren;
    }
  };  // namespace NodeValidationUtils
}  // namespace vrml_proc::traversor::validation
