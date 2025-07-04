#pragma once

namespace vrml_proc::traversor::node_descriptor {
  /**
   * @brief Represents all possible field types for NodeDescriptor.
   */
  enum class FieldType {
    Unknown,
    Bool,
    String,
    Float32,
    Int32,
    Vec2f,
    Vec3f,
    Vec4f,
    Vec2fArray,
    Vec3fArray,
    Int32Array,
    Node,
    NodeArray
  };
}  // namespace vrml_proc::traversor::node_descriptor
