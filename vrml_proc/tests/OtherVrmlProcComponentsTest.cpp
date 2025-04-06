#include <catch2/catch_test_macros.hpp>

#include <string>
#include <vector>

#include <NodeDescriptor.hpp>
#include <Int32Array.hpp>
#include <Logger.hpp>
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

TEST_CASE("NodeDescriptor - Valid", "[valid]") {  //

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
    CHECK(nd.GetId() == "VRML");
    CHECK(nd.GetAdditionalIds().size() == 0);
  }

  {
    vrml_proc::traversor::node_descriptor::NodeDescriptor nd("vrml", "A");
    CHECK(nd.GetId() == "vrml");
    CHECK(nd.GetAdditionalIds().size() == 1);
    CHECK(nd.GetAdditionalIds().find("A") != nd.GetAdditionalIds().end());
  }

  {
    vrml_proc::traversor::node_descriptor::NodeDescriptor nd("vrml", {"A", "B", "C"});
    CHECK(nd.GetId() == "vrml");
    CHECK(nd.GetAdditionalIds().size() == 3);
    CHECK(nd.GetAdditionalIds().find("A") != nd.GetAdditionalIds().end());
    CHECK(nd.GetAdditionalIds().find("B") != nd.GetAdditionalIds().end());
    CHECK(nd.GetAdditionalIds().find("C") != nd.GetAdditionalIds().end());
    CHECK(nd.GetAdditionalIds().find("D") == nd.GetAdditionalIds().end());
  }

  {
    vrml_proc::traversor::node_descriptor::NodeDescriptor nd("vrml");
    vrml_proc::parser::VrmlNode node;
    node.header = "Root";
    vrml_proc::parser::VrmlNodeManager manager;
    CHECK(nd.Validate(node, manager).has_value());
    nd = vrml_proc::traversor::node_descriptor::NodeDescriptor("vrml");
    CHECK(nd.Validate(node, manager, true).has_error());
  }

  // ------------------------------------------------------------------- //

  {  // Expected no fields.
    vrml_proc::traversor::node_descriptor::NodeDescriptor nd("Root");

    vrml_proc::parser::VrmlNode node;
    node.header = "Root";
    node.fields.push_back(f1);
    vrml_proc::parser::VrmlNodeManager manager;
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
    vrml_proc::parser::VrmlNodeManager manager;
    manager.AddDefinitionNode("ID", n);
    auto result = nd.Validate(node, manager);
    REQUIRE(result.has_value());
    CHECK(result.value()->GetId() == "Root");

    CHECK(result.value()->FieldExists("string"));
    CHECK_FALSE(result.value()->FieldExists("unknown"));
    CHECK(result.value()->GetFieldType("string") == vrml_proc::traversor::node_descriptor::FieldType::String);
    CHECK(result.value()->GetField<std::reference_wrapper<const std::string>>("string").get() == "value");
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
    vrml_proc::parser::VrmlNodeManager manager;
    manager.AddDefinitionNode("ID", n);
    auto result = nd.Validate(node, manager);
    REQUIRE(result.has_value());
    if (result.has_error()) LogError(result.error());

    CHECK(result.value()->FieldExists("string"));
    CHECK(result.value()->GetField<std::reference_wrapper<const std::string>>("string").get() == "");
  }
}
