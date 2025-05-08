#pragma once

#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

#include <result.hpp>

#include "Int32Array.hpp"
#include "NodeDescriptorFieldType.hpp"
#include "NodeValidationError.hpp"
#include "NodeValidationUtils.hpp"
#include "NodeView.hpp"
#include "Vec2f.hpp"
#include "Vec2fArray.hpp"
#include "Vec3f.hpp"
#include "Vec3fArray.hpp"
#include "Vec4f.hpp"
#include "VrmlField.hpp"
#include "VrmlNode.hpp"
#include "VrmlNodeManager.hpp"
#include "VrmlUnits.hpp"

namespace vrml_proc::traversor::node_descriptor {
  /**
   * @brief Represents a definition mechanism for VRML 2.0 nodes. Using `NodeDescriptor`, all node types are defined and
   * their default values are stored.
   */
  class NodeDescriptor {
   public:
    /**
     * @brief Creates new object.
     */
    NodeDescriptor() : m_name("") {}

    /**
     * @brief Creates new object.
     *
     * @param id name of the VRML node
     */
    NodeDescriptor(const std::string& id) : m_name(id) {}

    /**
     * @brief Creates new object.
     *
     * @param id name of the VRML node
     * @param synonym synonym to the canonical name in `id` variable
     */
    NodeDescriptor(const std::string& id, const std::string& synonym) : m_name(id), m_synonyms({synonym}) {}

    /**
     * @brief Creates new object.
     *
     * @param id name of the VRML node
     * @param synonyms synonyms to the canonical name in `id` variable
     */
    NodeDescriptor(const std::string& id, const std::unordered_set<std::string>& synonyms)
        : m_name(id), m_synonyms(synonyms) {}

    /**
     * @brief Binds a field to the node descriptor.
     *
     * @tparam Type type of the field
     * @param fieldName name of the new field
     * @param defaultValue default value of the field - the value is currently takes as reference
     * @note `defaultValue` has to exist as long as possible NodeView object using it
     *
     * @todo `defaultValue` value will not have to exist as long as now. Currently, it does, and it saves space, becuase
     * for every e.g. Group node, all default values for every such node are in the scene graph the same. That is
     * possible, because the tree is read-only.
     */
    template <typename Type>
    void BindField(const std::string& fieldName, const Type& defaultValue);

    /**
     * @brief Binds a VRML node as a field.
     *
     * @param fieldName name of the new field
     * @param validNodeHeaders valid names (node types) for the new node field
     * @param defaultNode default node
     *
     * @todo `defaultNode` does not make sense here, as default value should be just NULL - or just an empty node
     */
    void BindVrmlNode(const std::string& fieldName,
        const std::unordered_set<std::string>& validNodeHeaders,
        const vrml_proc::parser::VrmlNode& defaultNode) {
      m_fieldTypes[fieldName] = FieldType::Node;
      m_validHeaderNames[fieldName] = validNodeHeaders;
      m_defaultNodeFields[fieldName] = std::cref(defaultNode);
    }

    /**
     * @brief Binds a VRML node array
     *
     * @param fieldName name of the new field
     */
    void BindVrmlNodeArray(const std::string& fieldName) {
      m_fieldTypes[fieldName] = FieldType::NodeArray;
      m_defaultNodeArrayFields[fieldName] = std::vector<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>{};
    }

    /**
     * @brief Gets name (node type) of the node descriptor.
     *
     * @returns name
     */
    std::string GetName() const { return m_name; }

    /**
     * @brief Gets all synonyms.
     *
     * @returns all synonyms as set
     */
    const std::unordered_set<std::string>& GetSynonyms() const { return m_synonyms; }

    /**
     * @brief Validates given node `node` agains `this` node descriptor.
     *
     * @param node node to validate
     * @param manager manager
     * @param checkName flag indicating if the name of the `node` should be checked or not
     * @returns NodeView if the validation runs succesfully, otherwise NodeValidationError
     */
    cpp::result<std::shared_ptr<NodeView>,
        std::shared_ptr<vrml_proc::traversor::validation::error::NodeValidationError>>
    Validate(const vrml_proc::parser::VrmlNode& node,
        const vrml_proc::parser::VrmlNodeManager& manager,
        bool checkName = false) {  //

      using namespace vrml_proc::traversor::validation::NodeValidationUtils;

      if (checkName && !((node.header == m_name) || (m_synonyms.find(node.header) != m_synonyms.end()))) {
        auto expectedHeaders = m_synonyms;
        expectedHeaders.insert(m_name);
        return cpp::fail(std::make_shared<vrml_proc::traversor::validation::error::InvalidVrmlNodeHeader>(
            node.header, expectedHeaders));
      }

      NodeView::Builder builder;
      builder.SetName(m_name);
      builder.AddSynonyms(m_synonyms);
      builder.SetDefaultValues(m_fieldTypes, m_defaultBoolFields, m_defaultStringFields, m_defaultFloat32Fields,
          m_defaultInt32Fields, m_defaultVec2fFields, m_defaultVec3fFields, m_defaultVec4fFields,
          m_defaultVec2fArrayFields, m_defaultVec3fArrayFields, m_defaultInt32ArrayFields, m_defaultNodeFields,
          m_defaultNodeArrayFields);

      if (node.fields.empty()) {
        return builder.Build();
      }

      auto fieldsResult =
          CheckForOnlyUniqueAllowedFieldNames(ExtractFieldNames(m_fieldTypes), node.fields, node.header);
      if (fieldsResult.has_error()) {
        return cpp::fail(fieldsResult.error());
      }

      for (const auto& field : node.fields) {
        FieldType type = m_fieldTypes[field.name];
        switch (type) {
          case FieldType::Node:

          {
            auto vrmlNode = ExtractVrmlNodeWithValidation(field.name, node.fields, manager);
            if (vrmlNode.has_error()) {
              return cpp::fail(vrmlNode.error());
            }

            if (vrmlNode.value().has_value()) {
              auto headerResult = CheckForOnlyAllowedVrmlNodeHeaders(
                  m_validHeaderNames[field.name], vrmlNode.value().value().get(), field.name);
              if (headerResult.has_error()) {
                return cpp::fail(headerResult.error());
              }
            }
            builder.AddField(field.name, vrmlNode.value());
            m_defaultNodeFields[field.name] = vrmlNode.value();
          } break;

          case FieldType::NodeArray:

          {
            auto vrmlNodeArray = ExtractVrmlNodeArrayWithValidation(field.name, node.fields, manager, true);
            if (vrmlNodeArray.has_error()) {
              return cpp::fail(vrmlNodeArray.error());
            }
            builder.AddField(field.name, vrmlNodeArray.value());
            m_defaultNodeArrayFields[field.name] = vrmlNodeArray.value();
          } break;

          case FieldType::Vec3f:

          {
            auto vec3f = ExtractFieldByNameWithValidation<vrml_proc::parser::Vec3f>(field.name, node.fields);
            if (vec3f.has_error()) {
              return cpp::fail(vec3f.error());
            }
            builder.AddField(field.name, vec3f.value());
            m_defaultVec3fFields[field.name] = vec3f.value();
          } break;

          case FieldType::Vec3fArray:

          {
            auto value = ExtractFieldByNameWithValidation<vrml_proc::parser::Vec3fArray>(field.name, node.fields);
            if (value.has_error()) {
              return cpp::fail(value.error());
            }
            builder.AddField(field.name, value.value());
            m_defaultVec3fArrayFields[field.name] = value.value();
          } break;

          case FieldType::Int32Array:

          {
            auto value = ExtractFieldByNameWithValidation<vrml_proc::parser::Int32Array>(field.name, node.fields);
            if (value.has_error()) {
              return cpp::fail(value.error());
            }
            builder.AddField(field.name, value.value());
            m_defaultInt32ArrayFields[field.name] = value.value();
          } break;

          case FieldType::Float32:

          {
            auto float32_t = ExtractFieldByNameWithValidation<vrml_proc::parser::float32_t>(field.name, node.fields);
            if (float32_t.has_error()) {
              return cpp::fail(float32_t.error());
            }
            builder.AddField(field.name, float32_t.value());
            m_defaultFloat32Fields[field.name] = float32_t.value();
          } break;

          case FieldType::Int32:

          {
            auto int32 = ExtractFieldByNameWithValidation<int32_t>(field.name, node.fields);
            if (int32.has_error()) {
              return cpp::fail(int32.error());
            }
            builder.AddField(field.name, int32.value());
            m_defaultInt32Fields[field.name] = int32.value();
          } break;

          case FieldType::Vec2f:

          {
            auto vec2f = ExtractFieldByNameWithValidation<vrml_proc::parser::Vec2f>(field.name, node.fields);
            if (vec2f.has_error()) {
              return cpp::fail(vec2f.error());
            }
            builder.AddField(field.name, vec2f.value());
            m_defaultVec2fFields[field.name] = vec2f.value();
          } break;

          case FieldType::Vec4f:

          {
            auto vec4f = ExtractFieldByNameWithValidation<vrml_proc::parser::Vec4f>(field.name, node.fields);
            if (vec4f.has_error()) {
              return cpp::fail(vec4f.error());
            }
            builder.AddField(field.name, vec4f.value());
            m_defaultVec4fFields[field.name] = vec4f.value();
          } break;

          case FieldType::Vec2fArray:

          {
            auto value = ExtractFieldByNameWithValidation<vrml_proc::parser::Vec2fArray>(field.name, node.fields);
            if (value.has_error()) {
              return cpp::fail(value.error());
            }
            builder.AddField(field.name, value.value());
            m_defaultVec2fArrayFields[field.name] = value.value();
          } break;

          case FieldType::Bool:

          {
            auto boolean = ExtractFieldByNameWithValidation<bool>(field.name, node.fields);
            if (boolean.has_error()) {
              return cpp::fail(boolean.error());
            }
            builder.AddField(field.name, boolean.value());
            m_defaultBoolFields[field.name] = boolean.value();
          } break;

          case FieldType::String:

          {
            auto string = ExtractFieldByNameWithValidation<std::string>(field.name, node.fields);
            if (string.has_error()) {
              return cpp::fail(string.error());
            }
            builder.AddField(field.name, string.value());
            m_defaultStringFields[field.name] = string.value();
          } break;

          default:
            break;
        }
      }

      return builder.Build();
    }

   private:
    std::string m_name;
    std::unordered_set<std::string> m_synonyms;

    std::map<std::string, FieldType> m_fieldTypes;
    std::map<std::string, std::unordered_set<std::string>> m_validHeaderNames;

    std::map<std::string, std::optional<std::reference_wrapper<const bool>>> m_defaultBoolFields;
    std::map<std::string, std::optional<std::reference_wrapper<const std::string>>> m_defaultStringFields;
    std::map<std::string, std::optional<std::reference_wrapper<const vrml_proc::parser::float32_t>>>
        m_defaultFloat32Fields;
    std::map<std::string, std::optional<std::reference_wrapper<const int32_t>>> m_defaultInt32Fields;
    std::map<std::string, std::optional<std::reference_wrapper<const vrml_proc::parser::Vec2f>>> m_defaultVec2fFields;
    std::map<std::string, std::optional<std::reference_wrapper<const vrml_proc::parser::Vec3f>>> m_defaultVec3fFields;
    std::map<std::string, std::optional<std::reference_wrapper<const vrml_proc::parser::Vec4f>>> m_defaultVec4fFields;
    std::map<std::string, std::optional<std::reference_wrapper<const vrml_proc::parser::Vec2fArray>>>
        m_defaultVec2fArrayFields;
    std::map<std::string, std::optional<std::reference_wrapper<const vrml_proc::parser::Vec3fArray>>>
        m_defaultVec3fArrayFields;
    std::map<std::string, std::optional<std::reference_wrapper<const vrml_proc::parser::Int32Array>>>
        m_defaultInt32ArrayFields;
    std::map<std::string, std::optional<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>> m_defaultNodeFields;
    std::map<std::string, std::optional<std::vector<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>>>
        m_defaultNodeArrayFields;

    /**
     * @brief Extracts field names of the map.
     *
     * @param fieldTypes map, where key is name of the field and value is FieldType of that field
     * @returns all unique field names
     */
    static std::unordered_set<std::string> ExtractFieldNames(const std::map<std::string, FieldType>& fieldTypes) {
      std::unordered_set<std::string> fieldNames;
      for (const auto& [key, _] : fieldTypes) {
        fieldNames.insert(key);
      }
      return fieldNames;
    }
  };
}  // namespace vrml_proc::traversor::node_descriptor

template <>
inline void vrml_proc::traversor::node_descriptor::NodeDescriptor::BindField(
    const std::string& fieldName, const bool& defaultValue) {
  m_fieldTypes[fieldName] = FieldType::Bool;
  m_defaultBoolFields[fieldName] = std::cref(defaultValue);
}

template <>
inline void vrml_proc::traversor::node_descriptor::NodeDescriptor::BindField(
    const std::string& fieldName, const std::string& defaultValue) {
  m_fieldTypes[fieldName] = FieldType::String;
  m_defaultStringFields[fieldName] = std::cref(defaultValue);
}

template <>
inline void vrml_proc::traversor::node_descriptor::NodeDescriptor::BindField(
    const std::string& fieldName, const vrml_proc::parser::float32_t& defaultValue) {
  m_fieldTypes[fieldName] = FieldType::Float32;
  m_defaultFloat32Fields[fieldName] = std::cref(defaultValue);
}

template <>
inline void vrml_proc::traversor::node_descriptor::NodeDescriptor::BindField(
    const std::string& fieldName, const int32_t& defaultValue) {
  m_fieldTypes[fieldName] = FieldType::Int32;
  m_defaultInt32Fields[fieldName] = std::cref(defaultValue);
}

template <>
inline void vrml_proc::traversor::node_descriptor::NodeDescriptor::BindField(
    const std::string& fieldName, const vrml_proc::parser::Vec2f& defaultValue) {
  m_fieldTypes[fieldName] = FieldType::Vec2f;
  m_defaultVec2fFields[fieldName] = std::cref(defaultValue);
}

template <>
inline void vrml_proc::traversor::node_descriptor::NodeDescriptor::BindField(
    const std::string& fieldName, const vrml_proc::parser::Vec3f& defaultValue) {
  m_fieldTypes[fieldName] = FieldType::Vec3f;
  m_defaultVec3fFields[fieldName] = std::cref(defaultValue);
}

template <>
inline void vrml_proc::traversor::node_descriptor::NodeDescriptor::BindField(
    const std::string& fieldName, const vrml_proc::parser::Vec4f& defaultValue) {
  m_fieldTypes[fieldName] = FieldType::Vec4f;
  m_defaultVec4fFields[fieldName] = std::cref(defaultValue);
}

template <>
inline void vrml_proc::traversor::node_descriptor::NodeDescriptor::BindField(
    const std::string& fieldName, const vrml_proc::parser::Vec2fArray& defaultValue) {
  m_fieldTypes[fieldName] = FieldType::Vec2fArray;
  m_defaultVec2fArrayFields[fieldName] = std::cref(defaultValue);
}

template <>
inline void vrml_proc::traversor::node_descriptor::NodeDescriptor::BindField(
    const std::string& fieldName, const vrml_proc::parser::Vec3fArray& defaultValue) {
  m_fieldTypes[fieldName] = FieldType::Vec3fArray;
  m_defaultVec3fArrayFields[fieldName] = std::cref(defaultValue);
}

template <>
inline void vrml_proc::traversor::node_descriptor::NodeDescriptor::BindField(
    const std::string& fieldName, const vrml_proc::parser::Int32Array& defaultValue) {
  m_fieldTypes[fieldName] = FieldType::Int32Array;
  m_defaultInt32ArrayFields[fieldName] = std::cref(defaultValue);
}