#include <catch2/catch_test_macros.hpp>

#include <string>
#include <vector>

#include <Int32Array.hpp>
#include <Logger.hpp>
#include <NodeDescriptor.hpp>
#include <UseNode.hpp>
#include <Vec2f.hpp>
#include <Vec2fArray.hpp>
#include <Vec3f.hpp>
#include <Vec3fArray.hpp>
#include <Vec4f.hpp>
#include <VrmlField.hpp>
#include <VrmlNode.hpp>
#include <VrmlNodeManager.hpp>

#include "../../test_utils/TestCommon.hpp"

TEST_CASE("Initialization") { vrml_proc::core::logger::InitLogging(); }

TEST_CASE("NodeDescriptor", "NodeDescriptor") {  //

  vrml_proc::parser::VrmlField f1;
  f1.name = "string";
  f1.value = std::string("value");

  vrml_proc::parser::VrmlField f2;
  f2.name = "bool";
  f2.value = true;

  vrml_proc::parser::VrmlField f3;
  f3.name = "float32";
  f3.value = 5.0f;

  vrml_proc::parser::VrmlField f4;
  f4.name = "int32";
  f4.value = ((int32_t)8);

  vrml_proc::parser::VrmlField f5;
  f5.name = "vec2f";
  f5.value = vrml_proc::parser::Vec2f(0.0f, 1.0f);

  vrml_proc::parser::VrmlField f6;
  f6.name = "vec3f";
  f6.value = vrml_proc::parser::Vec3f(0.0f, 1.0f, 2.0f);

  vrml_proc::parser::VrmlField f7;
  f7.name = "vec4f";
  f7.value = vrml_proc::parser::Vec4f(0.0f, 1.0f, 2.0f, 3.0f);

  vrml_proc::parser::VrmlField f8;
  f8.name = "int32array";
  vrml_proc::parser::Int32Array a;
  a.integers.push_back(0);
  a.integers.push_back(1);
  f8.value = a;

  vrml_proc::parser::VrmlField f9;
  f9.name = "vec2farray";
  vrml_proc::parser::Vec2fArray aa;
  aa.vectors.push_back(vrml_proc::parser::Vec2f(0.0f, 1.0f));
  aa.vectors.push_back(vrml_proc::parser::Vec2f(0.0f, 1.0f));
  f9.value = aa;

  vrml_proc::parser::VrmlField f10;
  f10.name = "vec3farray";
  vrml_proc::parser::Vec3fArray aaa;
  aaa.vectors.push_back(vrml_proc::parser::Vec3f(0.0f, 1.0f, 2.0f));
  aaa.vectors.push_back(vrml_proc::parser::Vec3f(0.0f, 1.0f, 2.0f));
  f10.value = aaa;

  vrml_proc::parser::VrmlField f11;
  f11.name = "node";
  vrml_proc::parser::VrmlNode n;
  n.header = "child";
  n.definitionName = "ID";
  f11.value = n;

  vrml_proc::parser::VrmlField f12;
  f12.name = "node array";
  vrml_proc::parser::VrmlNode nn;
  vrml_proc::parser::UseNode u;
  u.identifier = "ID";
  vrml_proc::parser::VrmlNodeArray aaaa;
  aaaa.push_back(nn);
  aaaa.push_back(u);
  f12.value = aaaa;

  // ------------------------------------------------------------------- //

  {
    vrml_proc::traversor::node_descriptor::NodeDescriptor nd("VRML");
    CHECK(nd.GetName() == "VRML");
    CHECK(nd.GetSynonyms().size() == 0);
  }

  {
    vrml_proc::traversor::node_descriptor::NodeDescriptor nd("vrml", "A");
    CHECK(nd.GetName() == "vrml");
    CHECK(nd.GetSynonyms().size() == 1);
    CHECK(nd.GetSynonyms().find("A") != nd.GetSynonyms().end());
  }

  {
    vrml_proc::traversor::node_descriptor::NodeDescriptor nd("vrml", {"A", "B", "C"});
    CHECK(nd.GetName() == "vrml");
    CHECK(nd.GetSynonyms().size() == 3);
    CHECK(nd.GetSynonyms().find("A") != nd.GetSynonyms().end());
    CHECK(nd.GetSynonyms().find("B") != nd.GetSynonyms().end());
    CHECK(nd.GetSynonyms().find("C") != nd.GetSynonyms().end());
    CHECK(nd.GetSynonyms().find("D") == nd.GetSynonyms().end());
  }

  {
    vrml_proc::parser::service::VrmlNodeManager manager;
    vrml_proc::parser::VrmlNode node;
    node.header = "Root";

    vrml_proc::traversor::node_descriptor::NodeDescriptor nd("vrml");
    CHECK(nd.Validate(node, manager).has_value());

    nd = vrml_proc::traversor::node_descriptor::NodeDescriptor("vrml");
    CHECK(nd.Validate(node, manager, false).has_value());

    nd = vrml_proc::traversor::node_descriptor::NodeDescriptor("vrml");
    CHECK(nd.Validate(node, manager, true).has_error());
  }

  {
    vrml_proc::traversor::node_descriptor::NodeDescriptor nd(
        "vrml", std::unordered_set<std::string>{"vrml2.0", "vrml3.0"});
    vrml_proc::parser::VrmlNode node;
    node.header = "vrml2.0";
    vrml_proc::parser::service::VrmlNodeManager manager;
    CHECK(nd.Validate(node, manager, true).has_value());
    node.header = "vrml4.0";
    CHECK(nd.Validate(node, manager, true).has_error());
  }

  // ------------------------------------------------------------------- //

  {  // Expected no fields.
    vrml_proc::traversor::node_descriptor::NodeDescriptor nd("Root");

    vrml_proc::parser::VrmlNode node;
    node.header = "Root";
    node.fields.push_back(f1);
    vrml_proc::parser::service::VrmlNodeManager manager;
    auto result = nd.Validate(node, manager);
    REQUIRE(result.has_error());
    LogError(result.error());
  }

  {  // Valid case.
    vrml_proc::traversor::node_descriptor::NodeDescriptor nd("Root");
    std::string s = "";
    bool b = false;
    float f = 0.0f;
    int32_t i = 1;
    vrml_proc::parser::Vec2f v2;
    vrml_proc::parser::Vec3f v3;
    vrml_proc::parser::Vec4f v4;
    vrml_proc::parser::Vec3fArray v3a;
    vrml_proc::parser::Vec2fArray v2a;
    vrml_proc::parser::Int32Array ia;
    vrml_proc::parser::VrmlNode p;
    nd.BindField("string", s);
    nd.BindField("bool", b);
    nd.BindField("float32", f);
    nd.BindField("int32", i);
    nd.BindField("vec2f", v2);
    nd.BindField("vec3f", v3);
    nd.BindField("vec4f", v4);
    nd.BindField("vec2farray", v2a);
    nd.BindField("int32array", ia);
    nd.BindField("vec3farray", v3a);
    nd.BindVrmlNode("node", {"child"}, p);
    nd.BindVrmlNodeArray("node array");

    vrml_proc::parser::VrmlNode node;
    node.header = "Root";
    node.fields.push_back(f1);
    node.fields.push_back(f2);
    node.fields.push_back(f3);
    node.fields.push_back(f4);
    node.fields.push_back(f5);
    node.fields.push_back(f6);
    node.fields.push_back(f7);
    node.fields.push_back(f8);
    node.fields.push_back(f9);
    node.fields.push_back(f10);
    node.fields.push_back(f11);
    node.fields.push_back(f12);
    vrml_proc::parser::service::VrmlNodeManager manager;
    manager.AddDefinitionNode("ID", n);
    auto result = nd.Validate(node, manager);
    REQUIRE(result.has_value());
    CHECK(result.value()->GetName() == "Root");

    CHECK(result.value()->FieldExists("string"));
    CHECK_FALSE(result.value()->FieldExists("unknown"));

    CHECK(result.value()->GetFieldType("string") == vrml_proc::traversor::node_descriptor::FieldType::String);
    CHECK(result.value()->GetField<std::reference_wrapper<const std::string>>("string").get() == "value");

    CHECK(result.value()->GetFieldType("bool") == vrml_proc::traversor::node_descriptor::FieldType::Bool);
    CHECK(result.value()->GetField<std::reference_wrapper<const bool>>("bool").get() == true);

    CHECK(result.value()->GetFieldType("float32") == vrml_proc::traversor::node_descriptor::FieldType::Float32);
    CHECK(
        result.value()->GetField<std::reference_wrapper<const vrml_proc::parser::float32_t>>("float32").get() == 5.0f);

    CHECK(result.value()->GetFieldType("int32") == vrml_proc::traversor::node_descriptor::FieldType::Int32);
    CHECK(result.value()->GetField<std::reference_wrapper<const int32_t>>("int32").get() == 8);

    CHECK(result.value()->GetFieldType("vec2f") == vrml_proc::traversor::node_descriptor::FieldType::Vec2f);
    CHECK(result.value()->GetField<std::reference_wrapper<const vrml_proc::parser::Vec2f>>("vec2f").get().v == 1.0f);

    CHECK(result.value()->GetFieldType("vec3f") == vrml_proc::traversor::node_descriptor::FieldType::Vec3f);
    CHECK(result.value()->GetField<std::reference_wrapper<const vrml_proc::parser::Vec3f>>("vec3f").get().z == 2.0f);

    CHECK(result.value()->GetFieldType("vec4f") == vrml_proc::traversor::node_descriptor::FieldType::Vec4f);
    CHECK(result.value()->GetField<std::reference_wrapper<const vrml_proc::parser::Vec4f>>("vec4f").get().w == 3.0f);

    CHECK(result.value()->GetFieldType("int32array") == vrml_proc::traversor::node_descriptor::FieldType::Int32Array);
    {
      const auto& arr = result.value()
                            ->GetField<std::reference_wrapper<const vrml_proc::parser::Int32Array>>("int32array")
                            .get()
                            .integers;
      CHECK(arr.size() == 2);
      CHECK(arr[0] == 0);
      CHECK(arr[1] == 1);
    }

    CHECK(result.value()->GetFieldType("vec2farray") == vrml_proc::traversor::node_descriptor::FieldType::Vec2fArray);
    {
      const auto& arr = result.value()
                            ->GetField<std::reference_wrapper<const vrml_proc::parser::Vec2fArray>>("vec2farray")
                            .get()
                            .vectors;
      CHECK(arr.size() == 2);
      CHECK(arr[0].u == 0.0f);
      CHECK(arr[1].v == 1.0f);
    }

    CHECK(result.value()->GetFieldType("vec3farray") == vrml_proc::traversor::node_descriptor::FieldType::Vec3fArray);
    {
      const auto& arr = result.value()
                            ->GetField<std::reference_wrapper<const vrml_proc::parser::Vec3fArray>>("vec3farray")
                            .get()
                            .vectors;
      CHECK(arr.size() == 2);
      CHECK(arr[0].z == 2.0f);
      CHECK(arr[1].z == 2.0f);
    }

    CHECK(result.value()->GetFieldType("node") == vrml_proc::traversor::node_descriptor::FieldType::Node);
    {
      const auto& node =
          result.value()->GetField<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>("node").get();
      CHECK(node.header == "child");
      CHECK(node.definitionName.value() == "ID");
    }

    CHECK(result.value()->GetFieldType("node array") == vrml_proc::traversor::node_descriptor::FieldType::NodeArray);
    {
      const auto& nodes =
          result.value()->GetField<std::vector<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>>(
              "node array");
      CHECK(nodes.size() == 2);
    }

    CHECK_FALSE(result.value()->IsNodeShapeDescendant());
    result.value()->SetShapeDescendant(true);
    CHECK(result.value()->IsNodeShapeDescendant());

    vrml_proc::math::TransformationMatrix defaultMatrix;
    CHECK(result.value()->GetTransformationMatrix() == defaultMatrix);
    vrml_proc::math::Transformation data;
    data.center = vrml_proc::parser::Vec3f(5.0f, -8.0f, -10.5);
    auto updatedMatrix = vrml_proc::math::UpdateTransformationMatrix(defaultMatrix, data);
    result.value()->SetTransformationMatrix(updatedMatrix);
    CHECK(result.value()->GetTransformationMatrix() == updatedMatrix);
  }

  {  // Valid case, checking that default value is used.
    vrml_proc::traversor::node_descriptor::NodeDescriptor nd("Root");
    std::string s = "";
    bool b = false;
    float f = 0.0f;
    int32_t i = 1;
    vrml_proc::parser::Vec2f v2;
    vrml_proc::parser::Vec3f v3;
    vrml_proc::parser::Vec4f v4;
    vrml_proc::parser::Vec3fArray v3a;
    vrml_proc::parser::Vec2fArray v2a;
    vrml_proc::parser::Int32Array ia;
    vrml_proc::parser::VrmlNode p;
    nd.BindField("string", s);
    nd.BindField("bool", b);
    nd.BindField("float32", f);
    nd.BindField("int32", i);
    nd.BindField("vec2f", v2);
    nd.BindField("vec3f", v3);
    nd.BindField("vec4f", v4);
    nd.BindField("vec2farray", v2a);
    nd.BindField("int32array", ia);
    nd.BindField("vec3farray", v3a);
    nd.BindVrmlNode("node", {"child"}, p);
    nd.BindVrmlNodeArray("node array");

    vrml_proc::parser::VrmlNode node;
    node.header = "Root";
    node.fields.push_back(f2);
    node.fields.push_back(f3);
    node.fields.push_back(f4);
    node.fields.push_back(f5);
    node.fields.push_back(f6);
    node.fields.push_back(f7);
    node.fields.push_back(f8);
    node.fields.push_back(f9);
    node.fields.push_back(f10);
    node.fields.push_back(f11);
    node.fields.push_back(f12);
    vrml_proc::parser::service::VrmlNodeManager manager;
    manager.AddDefinitionNode("ID", n);
    auto result = nd.Validate(node, manager);
    REQUIRE(result.has_value());
    if (result.has_error()) LogError(result.error());

    CHECK(result.value()->FieldExists("string"));
    CHECK(result.value()->GetField<std::reference_wrapper<const std::string>>("string").get() == "");
  }
}
