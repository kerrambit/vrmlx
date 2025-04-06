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
#include "NodeValidationError.hpp"
#include "NodeValidationUtils.hpp"
#include "Vec2f.hpp"
#include "Vec2fArray.hpp"
#include "Vec3f.hpp"
#include "Vec3fArray.hpp"
#include "Vec4f.hpp"
#include "VrmlField.hpp"
#include "VrmlNode.hpp"
#include "VrmlNodeManager.hpp"
#include "VrmlUnits.hpp"
#include "NodeDescriptorFieldType.hpp"

namespace vrml_proc::traversor::node_descriptor {

  class NodeView {
   public:
    NodeView() = default;

    template <typename Type>
    Type GetField(const std::string& fieldName) const;

    bool FieldExists(const std::string& fieldName) const { return m_fieldTypes.find(fieldName) != m_fieldTypes.end(); }

    FieldType GetFieldType(const std::string& fieldName) const {
      if (FieldExists(fieldName)) {
        return m_fieldTypes.at(fieldName);
      }
      return FieldType::Unknown;
    }

    std::string GetId() const { return m_id; }

    const std::unordered_set<std::string>& GetAdditionalIds() const { return m_additionalIds; }

    class Builder;

   private:
    std::string m_id;
    std::unordered_set<std::string> m_additionalIds;

    std::map<std::string, FieldType> m_fieldTypes;
    std::map<std::string, std::unordered_set<std::string>> m_validHeaderNames;

    std::map<std::string, std::optional<std::reference_wrapper<const bool>>> m_boolFields;
    std::map<std::string, std::optional<std::reference_wrapper<const std::string>>> m_stringFields;
    std::map<std::string, std::optional<std::reference_wrapper<const vrml_proc::parser::float32_t>>> m_float32Fields;
    std::map<std::string, std::optional<std::reference_wrapper<const int32_t>>> m_int32Fields;
    std::map<std::string, std::optional<std::reference_wrapper<const vrml_proc::parser::Vec2f>>> m_vec2fFields;
    std::map<std::string, std::optional<std::reference_wrapper<const vrml_proc::parser::Vec3f>>> m_vec3fFields;
    std::map<std::string, std::optional<std::reference_wrapper<const vrml_proc::parser::Vec4f>>> m_vec4fFields;
    std::map<std::string, std::optional<std::reference_wrapper<const vrml_proc::parser::Vec2fArray>>>
        m_vec2fArrayFields;
    std::map<std::string, std::optional<std::reference_wrapper<const vrml_proc::parser::Vec3fArray>>>
        m_vec3fArrayFields;
    std::map<std::string, std::optional<std::reference_wrapper<const vrml_proc::parser::Int32Array>>>
        m_int32ArrayFields;
    std::map<std::string, std::optional<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>> m_nodeFields;
    std::map<std::string, std::optional<std::vector<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>>>
        m_nodeArrayFields;
  };

  class NodeView::Builder {
   public:
    Builder() : m_view(std::make_shared<NodeView>()) {}

    Builder& SetId(const std::string& id) {
      m_view->m_id = id;
      return *this;
    }

    Builder& SetDefaultValues(const std::map<std::string, FieldType>& fieldTypes,
        const std::map<std::string, std::optional<std::reference_wrapper<const bool>>>& boolFields,
        const std::map<std::string, std::optional<std::reference_wrapper<const std::string>>>& stringFields,
        const std::map<std::string, std::optional<std::reference_wrapper<const vrml_proc::parser::float32_t>>>&
            float32Fields,
        const std::map<std::string, std::optional<std::reference_wrapper<const int32_t>>>& int32Fields,
        const std::map<std::string, std::optional<std::reference_wrapper<const vrml_proc::parser::Vec2f>>>& vec2fFields,
        const std::map<std::string, std::optional<std::reference_wrapper<const vrml_proc::parser::Vec3f>>>& vec3fFields,
        const std::map<std::string, std::optional<std::reference_wrapper<const vrml_proc::parser::Vec4f>>>& vec4fFields,
        const std::map<std::string, std::optional<std::reference_wrapper<const vrml_proc::parser::Vec2fArray>>>&
            vec2fArrayFields,
        const std::map<std::string, std::optional<std::reference_wrapper<const vrml_proc::parser::Vec3fArray>>>&
            vec3fArrayFields,
        const std::map<std::string, std::optional<std::reference_wrapper<const vrml_proc::parser::Int32Array>>>&
            int32ArrayFields,
        const std::map<std::string, std::optional<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>>&
            nodeFields,
        const std::map<std::string,
            std::optional<std::vector<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>>>& nodeArrayFields) {
      m_view->m_fieldTypes = fieldTypes;
      m_view->m_boolFields = boolFields;
      m_view->m_stringFields = stringFields;
      m_view->m_float32Fields = float32Fields;
      m_view->m_int32Fields = int32Fields;
      m_view->m_vec2fFields = vec2fFields;
      m_view->m_vec3fFields = vec3fFields;
      m_view->m_vec4fFields = vec4fFields;
      m_view->m_vec2fArrayFields = vec2fArrayFields;
      m_view->m_vec3fArrayFields = vec3fArrayFields;
      m_view->m_int32ArrayFields = int32ArrayFields;
      m_view->m_nodeFields = nodeFields;
      m_view->m_nodeArrayFields = nodeArrayFields;

      return *this;
    }

    Builder& AddAdditionalId(const std::string& id) {
      m_view->m_additionalIds.insert(id);
      return *this;
    }

    Builder& AddAdditionalId(const std::unordered_set<std::string>& ids) {
      m_view->m_additionalIds.insert(ids.begin(), ids.end());
      return *this;
    }

    Builder& AddField(const std::string& name, std::optional<std::reference_wrapper<const bool>> value) {
      m_view->m_boolFields[name] = value;
      m_view->m_fieldTypes[name] = FieldType::Bool;
      return *this;
    }

    Builder& AddField(const std::string& name, std::optional<std::reference_wrapper<const std::string>> value) {
      m_view->m_stringFields[name] = value;
      m_view->m_fieldTypes[name] = FieldType::String;
      return *this;
    }

    Builder& AddField(
        const std::string& name, std::optional<std::reference_wrapper<const vrml_proc::parser::float32_t>> value) {
      m_view->m_float32Fields[name] = value;
      m_view->m_fieldTypes[name] = FieldType::Float32;
      return *this;
    }

    Builder& AddField(const std::string& name, std::optional<std::reference_wrapper<const int32_t>> value) {
      m_view->m_int32Fields[name] = value;
      m_view->m_fieldTypes[name] = FieldType::Int32;
      return *this;
    }

    Builder& AddField(
        const std::string& name, std::optional<std::reference_wrapper<const vrml_proc::parser::Vec2f>> value) {
      m_view->m_vec2fFields[name] = value;
      m_view->m_fieldTypes[name] = FieldType::Vec2f;
      return *this;
    }

    Builder& AddField(
        const std::string& name, std::optional<std::reference_wrapper<const vrml_proc::parser::Vec3f>> value) {
      m_view->m_vec3fFields[name] = value;
      m_view->m_fieldTypes[name] = FieldType::Vec3f;
      return *this;
    }

    Builder& AddField(
        const std::string& name, std::optional<std::reference_wrapper<const vrml_proc::parser::Vec4f>> value) {
      m_view->m_vec4fFields[name] = value;
      m_view->m_fieldTypes[name] = FieldType::Vec4f;
      return *this;
    }

    Builder& AddField(
        const std::string& name, std::optional<std::reference_wrapper<const vrml_proc::parser::Vec2fArray>> value) {
      m_view->m_vec2fArrayFields[name] = value;
      m_view->m_fieldTypes[name] = FieldType::Vec2fArray;
      return *this;
    }

    Builder& AddField(
        const std::string& name, std::optional<std::reference_wrapper<const vrml_proc::parser::Vec3fArray>> value) {
      m_view->m_vec3fArrayFields[name] = value;
      m_view->m_fieldTypes[name] = FieldType::Vec3fArray;
      return *this;
    }

    Builder& AddField(
        const std::string& name, std::optional<std::reference_wrapper<const vrml_proc::parser::Int32Array>> value) {
      m_view->m_int32ArrayFields[name] = value;
      m_view->m_fieldTypes[name] = FieldType::Int32Array;
      return *this;
    }

    Builder& AddField(
        const std::string& name, std::optional<std::reference_wrapper<const vrml_proc::parser::VrmlNode>> value) {
      m_view->m_nodeFields[name] = value;
      m_view->m_fieldTypes[name] = FieldType::Node;
      return *this;
    }

    Builder& AddField(const std::string& name,
        std::optional<
            std::reference_wrapper<const std::vector<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>>>
            value) {
      m_view->m_nodeArrayFields[name] = value;
      m_view->m_fieldTypes[name] = FieldType::NodeArray;
      return *this;
    }

    std::shared_ptr<NodeView> Build() { return m_view; }

   private:
    std::shared_ptr<NodeView> m_view;
  };
}  // namespace vrml_proc::traversor::node_descriptor

template <>
inline std::reference_wrapper<const bool>
vrml_proc::traversor::node_descriptor::NodeView::GetField<std::reference_wrapper<const bool>>(
    const std::string& fieldName) const {
  return m_boolFields.at(fieldName).value();
}

template <>
inline std::reference_wrapper<const std::string>
vrml_proc::traversor::node_descriptor::NodeView::GetField<std::reference_wrapper<const std::string>>(
    const std::string& fieldName) const {
  return m_stringFields.at(fieldName).value();
}

template <>
inline std::reference_wrapper<const vrml_proc::parser::float32_t>
vrml_proc::traversor::node_descriptor::NodeView::GetField<std::reference_wrapper<const vrml_proc::parser::float32_t>>(
    const std::string& fieldName) const {
  return m_float32Fields.at(fieldName).value();
}

template <>
inline std::reference_wrapper<const int32_t>
vrml_proc::traversor::node_descriptor::NodeView::GetField<std::reference_wrapper<const int32_t>>(
    const std::string& fieldName) const {
  return m_int32Fields.at(fieldName).value();
}

template <>
inline std::reference_wrapper<const vrml_proc::parser::Vec2f>
vrml_proc::traversor::node_descriptor::NodeView::GetField<std::reference_wrapper<const vrml_proc::parser::Vec2f>>(
    const std::string& fieldName) const {
  return m_vec2fFields.at(fieldName).value();
}

template <>
inline std::reference_wrapper<const vrml_proc::parser::Vec3f>
vrml_proc::traversor::node_descriptor::NodeView::GetField<std::reference_wrapper<const vrml_proc::parser::Vec3f>>(
    const std::string& fieldName) const {
  return m_vec3fFields.at(fieldName).value();
}

template <>
inline std::reference_wrapper<const vrml_proc::parser::Vec4f>
vrml_proc::traversor::node_descriptor::NodeView::GetField<std::reference_wrapper<const vrml_proc::parser::Vec4f>>(
    const std::string& fieldName) const {
  return m_vec4fFields.at(fieldName).value();
}

template <>
inline std::reference_wrapper<const vrml_proc::parser::Vec2fArray>
vrml_proc::traversor::node_descriptor::NodeView::GetField<std::reference_wrapper<const vrml_proc::parser::Vec2fArray>>(
    const std::string& fieldName) const {
  return m_vec2fArrayFields.at(fieldName).value();
}

template <>
inline std::reference_wrapper<const vrml_proc::parser::Vec3fArray>
vrml_proc::traversor::node_descriptor::NodeView::GetField<std::reference_wrapper<const vrml_proc::parser::Vec3fArray>>(
    const std::string& fieldName) const {
  return m_vec3fArrayFields.at(fieldName).value();
}

template <>
inline std::reference_wrapper<const vrml_proc::parser::Int32Array>
vrml_proc::traversor::node_descriptor::NodeView::GetField<std::reference_wrapper<const vrml_proc::parser::Int32Array>>(
    const std::string& fieldName) const {
  return m_int32ArrayFields.at(fieldName).value();
}

template <>
inline std::reference_wrapper<const vrml_proc::parser::VrmlNode>
vrml_proc::traversor::node_descriptor::NodeView::GetField<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>(
    const std::string& fieldName) const {
  return m_nodeFields.at(fieldName).value();
}

template <>
inline std::vector<std::reference_wrapper<const vrml_proc::parser::VrmlNode>> vrml_proc::traversor::node_descriptor::
    NodeView::GetField<std::vector<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>>(
        const std::string& fieldName) const {
  return m_nodeArrayFields.at(fieldName).value();
}
