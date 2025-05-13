#include <catch2/catch_test_macros.hpp>

#include <cstdint>
#include <string>
#include <type_traits>
#include <vector>

#include <boost/variant/get.hpp>
#include <boost/variant/recursive_wrapper.hpp>
#include <boost/variant/variant.hpp>

#include "test_data/VrmlFieldExtractorTestDataset.hpp"
#include <Int32Array.hpp>
#include <Logger.hpp>
#include <ParserResult.hpp>
#include <UseNode.hpp>
#include <Vec2f.hpp>
#include <Vec2fArray.hpp>
#include <Vec3f.hpp>
#include <Vec3fArray.hpp>
#include <Vec4f.hpp>
#include <VrmlField.hpp>
#include <VrmlFieldExtractor.hpp>
#include <VrmlFile.hpp>
#include <VrmlNode.hpp>
#include <VrmlNodeManager.hpp>
#include <VrmlParser.hpp>

#include "../../test_utils/TestCommon.hpp"

TEST_CASE("Initialization") { vrml_proc::core::logger::InitLogging(); }

TEST_CASE("IsNamePresent - Valid", "[valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(first, manager);
  REQUIRE(parseResult);

  auto& fields = parseResult.value().at(0).fields;

  CHECK(vrml_proc::parser::model::utils::VrmlFieldExtractor::IsNamePresent("field1", fields));
  CHECK(vrml_proc::parser::model::utils::VrmlFieldExtractor::IsNamePresent("field2", fields));
}

TEST_CASE("IsNamePresent - Inalid", "[invalid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(first, manager);
  REQUIRE(parseResult);

  auto& fields = parseResult.value().at(0).fields;

  CHECK_FALSE(vrml_proc::parser::model::utils::VrmlFieldExtractor::IsNamePresent("field3", fields));
  CHECK_FALSE(vrml_proc::parser::model::utils::VrmlFieldExtractor::IsNamePresent("color", fields));
}

TEST_CASE("ExtractByNameExtended - Missing Field", "[invalid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(nodesArray, manager);
  REQUIRE(parseResult);

  auto& fields = parseResult.value().at(0).fields;

  std::string invalidType;
  auto result =
      vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<vrml_proc::parser::model::Int32Array>(
          "light", fields, invalidType);
  REQUIRE(result.has_error());
  CHECK(result.error() == vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameError::FieldNotFound);
}

TEST_CASE("ExtractByNameExtended - Validation Error", "[invalid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(nodesArray, manager);
  REQUIRE(parseResult);

  auto& fields = parseResult.value().at(0).fields;

  std::string invalidType;
  auto result =
      vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<vrml_proc::parser::model::Int32Array>(
          "shape", fields, invalidType);
  REQUIRE(result.has_error());
  CHECK(result.error() == vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameError::ValidationError);
  CHECK(invalidType.find("vrml_proc::parser::model::VrmlNode") != std::string::npos);
}

TEST_CASE("ExtractByNameExtended - VrmlNode Non-recursive I.", "[valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(nodesArray, manager);
  REQUIRE(parseResult);

  auto& fieldsOfRoot = parseResult.value().at(0).fields;
  vrml_proc::parser::model::VrmlNode* ptr = boost::get<vrml_proc::parser::model::VrmlNode>(&fieldsOfRoot.at(0).value);

  std::string invalidType;
  auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<vrml_proc::parser::model::VrmlNode>(
      "shape", fieldsOfRoot, invalidType);
  REQUIRE(result.has_value());
  CHECK(ptr == &(result.value().get()));
}

TEST_CASE("ExtractByNameExtended - VrmlNode Non-recursive II.", "[valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(nodesArray, manager);
  REQUIRE(parseResult);

  auto& fieldsOfRoot = parseResult.value().at(0).fields;
  vrml_proc::parser::model::VrmlNode& shape = boost::get<vrml_proc::parser::model::VrmlNode>(fieldsOfRoot.at(0).value);
  auto& fieldsOfShape = shape.fields;
  vrml_proc::parser::model::VrmlNode* ptr = boost::get<vrml_proc::parser::model::VrmlNode>(&fieldsOfShape.at(0).value);

  std::string invalidType;
  auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<vrml_proc::parser::model::VrmlNode>(
      "geometry", fieldsOfShape, invalidType);
  REQUIRE(result.has_value());
  CHECK(ptr == &(result.value().get()));
}

TEST_CASE("ExtractByNameExtended - VrmlNode Array", "[valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(children, manager);
  REQUIRE(parseResult);

  auto& fieldsOfRoot = parseResult.value().at(0).fields;
  std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::model::VrmlNode>,
      boost::recursive_wrapper<vrml_proc::parser::model::UseNode>>>* ptr =
      boost::get<std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::model::VrmlNode>,
          boost::recursive_wrapper<vrml_proc::parser::model::UseNode>>>>(&fieldsOfRoot.at(0).value);

  boost::variant<boost::recursive_wrapper<vrml_proc::parser::model::VrmlNode>,
      boost::recursive_wrapper<vrml_proc::parser::model::UseNode>>* firstOfChildrenPtr = &ptr->at(0);

  std::string invalidType;
  auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<std::vector<boost::variant<
      boost::recursive_wrapper<vrml_proc::parser::model::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::model::UseNode>>>>(
      "children", fieldsOfRoot, invalidType);
  REQUIRE(result.has_value());
  CHECK(ptr == &(result.value().get()));
  CHECK(firstOfChildrenPtr == &(result.value().get().at(0)));
}

TEST_CASE("ExtractByNameExtended - Empty Array of Vec2f's", "[valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(emptyArray, manager);
  REQUIRE(parseResult);

  auto& fieldsOfRoot = parseResult.value().at(0).fields;

  std::string invalidType;
  auto result =
      vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<vrml_proc::parser::model::Vec2fArray>(
          "array", fieldsOfRoot, invalidType);
  REQUIRE(result.has_value());
  CHECK(result.value().get().vectors.size() == 0);
}

TEST_CASE("ExtractByNameExtended - Empty Array of Vec3f's", "[valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(emptyArray, manager);
  REQUIRE(parseResult);

  auto& fieldsOfRoot = parseResult.value().at(0).fields;
  vrml_proc::parser::model::Vec3fArray* ptr = boost::get<vrml_proc::parser::model::Vec3fArray>(&fieldsOfRoot.at(0).value);

  std::string invalidType;
  auto result =
      vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<vrml_proc::parser::model::Vec3fArray>(
          "array", fieldsOfRoot, invalidType);
  REQUIRE(result.has_value());
  CHECK(ptr == &(result.value().get()));
}

TEST_CASE("ExtractByNameExtended - Empty Array of Int32's", "[valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(emptyArray, manager);
  REQUIRE(parseResult);

  auto& fieldsOfRoot = parseResult.value().at(0).fields;

  std::string invalidType;
  auto result =
      vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<vrml_proc::parser::model::Int32Array>(
          "array", fieldsOfRoot, invalidType);
  REQUIRE(result.has_value());
  CHECK(result.value().get().integers.size() == 0);
}

TEST_CASE("ExtractByNameExtended - Empty Array of VRML nodes", "[valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(emptyArray, manager);
  REQUIRE(parseResult);

  auto& fieldsOfRoot = parseResult.value().at(0).fields;

  std::string invalidType;
  auto result =
      vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<vrml_proc::parser::model::VrmlNodeArray>(
          "array", fieldsOfRoot, invalidType);
  REQUIRE(result.has_value());
  CHECK(result.value().get().size() == 0);
}

TEST_CASE("ExtractByNameExtended - String", "[valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(simpleEntityTypes, manager);
  REQUIRE(parseResult);

  auto& fieldsOfRoot = parseResult.value().at(0).fields;
  std::string* ptr = boost::get<std::string>(&fieldsOfRoot.at(0).value);

  std::string invalidType;
  auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<std::string>(
      "string", fieldsOfRoot, invalidType);
  REQUIRE(result.has_value());
  CHECK(ptr == &(result.value().get()));
}

TEST_CASE("ExtractByNameExtended - Bool", "[valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(simpleEntityTypes, manager);
  REQUIRE(parseResult);

  auto& fieldsOfRoot = parseResult.value().at(0).fields;
  bool* ptr = boost::get<bool>(&fieldsOfRoot.at(1).value);

  std::string invalidType;
  auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<bool>(
      "bool", fieldsOfRoot, invalidType);
  REQUIRE(result.has_value());
  CHECK(ptr == &(result.value().get()));
}

TEST_CASE("ExtractByNameExtended - Vec3f", "[valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(simpleEntityTypes, manager);
  REQUIRE(parseResult);

  auto& fieldsOfRoot = parseResult.value().at(0).fields;
  vrml_proc::parser::model::Vec3f* ptr = boost::get<vrml_proc::parser::model::Vec3f>(&fieldsOfRoot.at(2).value);

  std::string invalidType;
  auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<vrml_proc::parser::model::Vec3f>(
      "vec3f", fieldsOfRoot, invalidType);
  REQUIRE(result.has_value());
  CHECK(ptr == &(result.value().get()));
}

TEST_CASE("ExtractByNameExtended - Vec3fArray", "[valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(simpleEntityTypes, manager);
  REQUIRE(parseResult);

  auto& fieldsOfRoot = parseResult.value().at(0).fields;
  vrml_proc::parser::model::Vec3fArray* ptr = boost::get<vrml_proc::parser::model::Vec3fArray>(&fieldsOfRoot.at(3).value);

  std::string invalidType;
  auto result =
      vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<vrml_proc::parser::model::Vec3fArray>(
          "vec3farray", fieldsOfRoot, invalidType);
  REQUIRE(result.has_value());
  CHECK(ptr == &(result.value().get()));
}

TEST_CASE("ExtractByNameExtended - Vec4", "[valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(simpleEntityTypes, manager);
  REQUIRE(parseResult);

  auto& fieldsOfRoot = parseResult.value().at(0).fields;
  vrml_proc::parser::model::Vec4f* ptr = boost::get<vrml_proc::parser::model::Vec4f>(&fieldsOfRoot.at(4).value);

  std::string invalidType;
  auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<vrml_proc::parser::model::Vec4f>(
      "vec4f", fieldsOfRoot, invalidType);
  REQUIRE(result.has_value());
  CHECK(ptr == &(result.value().get()));
}

TEST_CASE("ExtractByNameExtended - Int32Array", "[valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(simpleEntityTypes, manager);
  REQUIRE(parseResult);

  auto& fieldsOfRoot = parseResult.value().at(0).fields;
  vrml_proc::parser::model::Int32Array* ptr = boost::get<vrml_proc::parser::model::Int32Array>(&fieldsOfRoot.at(5).value);

  std::string invalidType;
  auto result =
      vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<vrml_proc::parser::model::Int32Array>(
          "int32array", fieldsOfRoot, invalidType);
  REQUIRE(result.has_value());
  CHECK(ptr == &(result.value().get()));
}

TEST_CASE("ExtractByNameExtended - float", "[valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(simpleEntityTypes, manager);
  REQUIRE(parseResult);

  auto& fieldsOfRoot = parseResult.value().at(0).fields;
  float* ptr = boost::get<float>(&fieldsOfRoot.at(6).value);

  std::string invalidType;
  auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<float>(
      "float", fieldsOfRoot, invalidType);
  REQUIRE(result.has_value());
  CHECK(ptr == &(result.value().get()));
}

TEST_CASE("ExtractByNameExtended - int32", "[valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(simpleEntityTypes, manager);
  REQUIRE(parseResult);

  auto& fieldsOfRoot = parseResult.value().at(0).fields;
  int32_t* ptr = boost::get<int32_t>(&fieldsOfRoot.at(7).value);

  std::string invalidType;
  auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<int32_t>(
      "int32", fieldsOfRoot, invalidType);
  REQUIRE(result.has_value());
  CHECK(ptr == &(result.value().get()));
}

TEST_CASE("ExtractByNameExtended - Vec2f", "[valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(simpleEntityTypes, manager);
  REQUIRE(parseResult);

  auto& fieldsOfRoot = parseResult.value().at(0).fields;
  vrml_proc::parser::model::Vec2f* ptr = boost::get<vrml_proc::parser::model::Vec2f>(&fieldsOfRoot.at(8).value);

  std::string invalidType;
  auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<vrml_proc::parser::model::Vec2f>(
      "vec2f", fieldsOfRoot, invalidType);
  REQUIRE(result.has_value());
  CHECK(ptr == &(result.value().get()));
}

TEST_CASE("ExtractByNameExtended - Vec2fArray", "[valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(simpleEntityTypes, manager);
  REQUIRE(parseResult);

  auto& fieldsOfRoot = parseResult.value().at(0).fields;
  vrml_proc::parser::model::Vec2fArray* ptr = boost::get<vrml_proc::parser::model::Vec2fArray>(&fieldsOfRoot.at(9).value);
  vrml_proc::parser::model::Vec2f* vecPtr = &ptr->vectors.at(0);
  vrml_proc::parser::model::float32_t* floatPtr = &vecPtr->u;

  std::string invalidType;
  auto result =
      vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<vrml_proc::parser::model::Vec2fArray>(
          "vec2farray", fieldsOfRoot, invalidType);
  REQUIRE(result.has_value());
  CHECK(ptr == &(result.value().get()));
  CHECK(vecPtr == &(result.value().get().vectors.at(0)));
  CHECK(floatPtr == &(result.value().get().vectors.at(0).u));
}

TEST_CASE("ExtractByNameExtended - UseNode", "[valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(nodesArray, manager);
  REQUIRE(parseResult);

  auto& fieldsOfRoot = parseResult.value().at(0).fields;
  vrml_proc::parser::model::VrmlNode& shape = boost::get<vrml_proc::parser::model::VrmlNode>(fieldsOfRoot.at(0).value);
  vrml_proc::parser::model::UseNode* ptr = boost::get<vrml_proc::parser::model::UseNode>(&shape.fields.at(2).value);

  std::string invalidType;
  auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<vrml_proc::parser::model::UseNode>(
      "validUseId", shape.fields, invalidType);
  REQUIRE(result.has_value());
  CHECK(ptr == &(result.value().get()));
}

TEST_CASE("ExtractVrmlNodeFromVariantWithoutResolvingExtended - Invalid I.", "[invalid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(children, manager);
  REQUIRE(parseResult);

  auto& fieldsOfRoot = parseResult.value().at(0).fields;
  std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::model::VrmlNode>,
      boost::recursive_wrapper<vrml_proc::parser::model::UseNode>>>& children =
      boost::get<std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::model::VrmlNode>,
          boost::recursive_wrapper<vrml_proc::parser::model::UseNode>>>>(fieldsOfRoot.at(0).value);
  vrml_proc::parser::model::VrmlNode& node = boost::get<vrml_proc::parser::model::VrmlNode>(children.at(0));

  std::string invalidType;
  const boost::variant<boost::recursive_wrapper<vrml_proc::parser::model::VrmlNode>,
      boost::recursive_wrapper<vrml_proc::parser::model::UseNode>>& variant = children.at(0);
  auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeFromVariantWithoutResolvingExtended<
      boost::recursive_wrapper<vrml_proc::parser::model::UseNode>>(variant, invalidType);
  REQUIRE(!result.has_value());
  CHECK(invalidType.find("vrml_proc::parser::model::VrmlNode") != std::string::npos);
}

TEST_CASE("ExtractVrmlNodeFromVariantWithoutResolvingExtended - Invalid II.", "[invalid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(children, manager);
  REQUIRE(parseResult);

  auto& fieldsOfRoot = parseResult.value().at(0).fields;
  std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::model::VrmlNode>,
      boost::recursive_wrapper<vrml_proc::parser::model::UseNode>>>& children =
      boost::get<std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::model::VrmlNode>,
          boost::recursive_wrapper<vrml_proc::parser::model::UseNode>>>>(fieldsOfRoot.at(0).value);

  std::string invalidType;
  const boost::variant<boost::recursive_wrapper<vrml_proc::parser::model::VrmlNode>,
      boost::recursive_wrapper<vrml_proc::parser::model::UseNode>>& variant = children.at(1);
  auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeFromVariantWithoutResolvingExtended<
      vrml_proc::parser::model::VrmlNode>(variant, invalidType);
  REQUIRE(!result.has_value());
  CHECK(invalidType.find("vrml_proc::parser::model::UseNode") != std::string::npos);
}

TEST_CASE("ExtractVrmlNodeFromVariantWithoutResolvingExtended - VrmlNode", "[valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(children, manager);
  REQUIRE(parseResult);

  auto& fieldsOfRoot = parseResult.value().at(0).fields;
  std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::model::VrmlNode>,
      boost::recursive_wrapper<vrml_proc::parser::model::UseNode>>>& children =
      boost::get<std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::model::VrmlNode>,
          boost::recursive_wrapper<vrml_proc::parser::model::UseNode>>>>(fieldsOfRoot.at(0).value);
  vrml_proc::parser::model::VrmlNode& node = boost::get<vrml_proc::parser::model::VrmlNode>(children.at(0));
  vrml_proc::parser::model::VrmlNode* ptr = &node;

  std::string invalidType;
  const boost::variant<boost::recursive_wrapper<vrml_proc::parser::model::VrmlNode>,
      boost::recursive_wrapper<vrml_proc::parser::model::UseNode>>& variant = children.at(0);
  auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeFromVariantWithoutResolvingExtended<
      vrml_proc::parser::model::VrmlNode>(variant, invalidType);
  REQUIRE(result.has_value());
  CHECK(&result.value().get() == ptr);
}

TEST_CASE("ExtractVrmlNodeFromVariantWithoutResolvingExtended - UseNode", "[valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(children, manager);
  REQUIRE(parseResult);

  auto& fieldsOfRoot = parseResult.value().at(0).fields;
  std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::model::VrmlNode>,
      boost::recursive_wrapper<vrml_proc::parser::model::UseNode>>>& children =
      boost::get<std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::model::VrmlNode>,
          boost::recursive_wrapper<vrml_proc::parser::model::UseNode>>>>(fieldsOfRoot.at(0).value);
  vrml_proc::parser::model::UseNode& node = boost::get<vrml_proc::parser::model::UseNode>(children.at(1));
  vrml_proc::parser::model::UseNode* ptr = &node;

  std::string invalidType;
  const boost::variant<boost::recursive_wrapper<vrml_proc::parser::model::VrmlNode>,
      boost::recursive_wrapper<vrml_proc::parser::model::UseNode>>& variant = children.at(1);
  auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeFromVariantWithoutResolvingExtended<
      vrml_proc::parser::model::UseNode>(variant, invalidType);
  REQUIRE(result.has_value());
  CHECK(&result.value().get() == ptr);
}

TEST_CASE("ExtractVrmlNodeFromVariantExtended - VrmlNode", "[valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(children, manager);
  REQUIRE(parseResult);

  auto& fieldsOfRoot = parseResult.value().at(0).fields;
  std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::model::VrmlNode>,
      boost::recursive_wrapper<vrml_proc::parser::model::UseNode>>>& children =
      boost::get<std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::model::VrmlNode>,
          boost::recursive_wrapper<vrml_proc::parser::model::UseNode>>>>(fieldsOfRoot.at(0).value);
  vrml_proc::parser::model::VrmlNode& node = boost::get<vrml_proc::parser::model::VrmlNode>(children.at(0));
  vrml_proc::parser::model::VrmlNode* ptr = &node;

  const boost::variant<boost::recursive_wrapper<vrml_proc::parser::model::VrmlNode>,
      boost::recursive_wrapper<vrml_proc::parser::model::UseNode>>& variant = children.at(0);

  std::string invalidType;
  std::string useId;
  auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeFromVariantExtended(
      variant, manager, invalidType, useId);
  REQUIRE(result.has_value());
  std::reference_wrapper<const vrml_proc::parser::model::VrmlNode>& wrapper = result.value();
  CHECK(&wrapper.get() == ptr);
}

TEST_CASE("ExtractVrmlNodeFromVariantExtended - UseNode", "[valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(children, manager);
  REQUIRE(parseResult);

  auto& fieldsOfRoot = parseResult.value().at(0).fields;
  std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::model::VrmlNode>,
      boost::recursive_wrapper<vrml_proc::parser::model::UseNode>>>& children =
      boost::get<std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::model::VrmlNode>,
          boost::recursive_wrapper<vrml_proc::parser::model::UseNode>>>>(fieldsOfRoot.at(0).value);

  const boost::variant<boost::recursive_wrapper<vrml_proc::parser::model::VrmlNode>,
      boost::recursive_wrapper<vrml_proc::parser::model::UseNode>>& variant = children.at(1);

  std::string invalidType;
  std::string useId;
  auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeFromVariantExtended(
      variant, manager, invalidType, useId);
  REQUIRE(result.has_value());
  std::reference_wrapper<const vrml_proc::parser::model::VrmlNode>& wrapper = result.value();
  CHECK(&wrapper.get() == &(manager.GetDefinitionNode("id").value().get()));
}

TEST_CASE("ExtractVrmlNodeFromVariantExtended - UseNode Is Missing", "[invalid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(children, manager);
  REQUIRE(parseResult);

  auto& fieldsOfRoot = parseResult.value().at(0).fields;
  std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::model::VrmlNode>,
      boost::recursive_wrapper<vrml_proc::parser::model::UseNode>>>& children =
      boost::get<std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::model::VrmlNode>,
          boost::recursive_wrapper<vrml_proc::parser::model::UseNode>>>>(fieldsOfRoot.at(0).value);

  const boost::variant<boost::recursive_wrapper<vrml_proc::parser::model::VrmlNode>,
      boost::recursive_wrapper<vrml_proc::parser::model::UseNode>>& variant = children.at(2);

  std::string invalidType;
  std::string useId;
  auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeFromVariantExtended(
      variant, manager, invalidType, useId);
  REQUIRE(result.has_error());
  CHECK(result.error() ==
        vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeFromVariantError::UnknownUseNode);
  CHECK(useId == "missingId");
}

TEST_CASE("ExtractVrmlNodeExtended - Field Not Found", "[invalid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(nodesArray, manager);
  REQUIRE(parseResult);

  auto& fieldsOfRoot = parseResult.value().at(0).fields;
  vrml_proc::parser::model::VrmlNode& shape = boost::get<vrml_proc::parser::model::VrmlNode>(fieldsOfRoot.at(0).value);

  std::string invalidType;
  std::string useId;
  auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeExtended(
      "logic", shape.fields, manager, invalidType, useId);
  REQUIRE(result.has_error());
  CHECK(result.error() == vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeError::FieldNotFound);
}

TEST_CASE("ExtractVrmlNodeExtended - Validation Error", "[invalid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(nodesArray, manager);
  REQUIRE(parseResult);

  auto& fieldsOfRoot = parseResult.value().at(0).fields;
  vrml_proc::parser::model::VrmlNode& shape = boost::get<vrml_proc::parser::model::VrmlNode>(fieldsOfRoot.at(0).value);

  std::string invalidType;
  std::string useId;
  auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeExtended(
      "appearance", shape.fields, manager, invalidType, useId);
  REQUIRE(result.has_error());
  CHECK(result.error() == vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeError::ValidationError);
  CHECK(invalidType.find("vrml_proc::parser::model::Vec4f") != std::string::npos);
}

TEST_CASE("ExtractVrmlNodeExtended - Missing USE Id Error", "[invalid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(nodesArray, manager);
  REQUIRE(parseResult);

  auto& fieldsOfRoot = parseResult.value().at(0).fields;
  vrml_proc::parser::model::VrmlNode& shape = boost::get<vrml_proc::parser::model::VrmlNode>(fieldsOfRoot.at(0).value);

  std::string invalidType;
  std::string useId;
  auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeExtended(
      "invalidUseId", shape.fields, manager, invalidType, useId);
  REQUIRE(result.has_error());
  CHECK(result.error() == vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeError::UnknownUseNode);
  CHECK(useId == "idd");
}

TEST_CASE("ExtractVrmlNodeExtended - USENode", "[valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(nodesArray, manager);
  REQUIRE(parseResult);

  auto& fieldsOfRoot = parseResult.value().at(0).fields;
  vrml_proc::parser::model::VrmlNode& shape = boost::get<vrml_proc::parser::model::VrmlNode>(fieldsOfRoot.at(0).value);

  std::string invalidType;
  std::string useId;
  auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeExtended(
      "validUseId", shape.fields, manager, invalidType, useId);
  REQUIRE(result.has_value());
  CHECK(&(manager.GetDefinitionNode("id").value().get()) == &(result.value().get()));
}

TEST_CASE("ExtractVrmlNodeExtended - VrmlNode", "[valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(nodesArray, manager);
  REQUIRE(parseResult);

  auto& fieldsOfRoot = parseResult.value().at(0).fields;
  vrml_proc::parser::model::VrmlNode& shape = boost::get<vrml_proc::parser::model::VrmlNode>(fieldsOfRoot.at(0).value);
  vrml_proc::parser::model::VrmlNode& geometry = boost::get<vrml_proc::parser::model::VrmlNode>(shape.fields.at(0).value);

  std::string invalidType;
  std::string useId;
  auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeExtended(
      "geometry", shape.fields, manager, invalidType, useId);
  REQUIRE(result.has_value());
  CHECK(&(result.value().get()) == &geometry);
}

TEST_CASE("ExtractByNameExtended - Edge cases - Int as int", "[valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(floatIntEdgeCase, manager);
  REQUIRE(parseResult);

  auto& fieldsOfRoot = parseResult.value().at(0).fields;
  int32_t* ptr = boost::get<int32_t>(&fieldsOfRoot.at(0).value);

  std::string invalidType;
  auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<int32_t>(
      "int", fieldsOfRoot, invalidType);
  REQUIRE(result.has_value());
  CHECK(ptr == &(result.value().get()));
}

TEST_CASE("ExtractByNameExtended - Edge cases - Int as float", "[valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(floatIntEdgeCase, manager);
  REQUIRE(parseResult);

  auto& fieldsOfRoot = parseResult.value().at(0).fields;
  vrml_proc::parser::model::float32_t* ptr = boost::get<vrml_proc::parser::model::float32_t>(&fieldsOfRoot.at(0).value);

  std::string invalidType;
  auto result =
      vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<vrml_proc::parser::model::float32_t>(
          "int", fieldsOfRoot, invalidType);
  REQUIRE(result.has_value());
}

TEST_CASE("ExtractByNameExtended - Edge cases - Float without decimal as int", "[valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(floatIntEdgeCase, manager);
  REQUIRE(parseResult);

  auto& fieldsOfRoot = parseResult.value().at(0).fields;
  int32_t* ptr = boost::get<int32_t>(&fieldsOfRoot.at(1).value);

  std::string invalidType;
  auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<int32_t>(
      "float_without_decimals", fieldsOfRoot, invalidType);
  REQUIRE(result.has_value());
}

TEST_CASE("ExtractByNameExtended - Edge cases - Float without decimal as float", "[valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(floatIntEdgeCase, manager);
  REQUIRE(parseResult);

  auto& fieldsOfRoot = parseResult.value().at(0).fields;
  vrml_proc::parser::model::float32_t* ptr = boost::get<vrml_proc::parser::model::float32_t>(&fieldsOfRoot.at(1).value);

  std::string invalidType;
  auto result =
      vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<vrml_proc::parser::model::float32_t>(
          "float_without_decimals", fieldsOfRoot, invalidType);
  REQUIRE(result.has_value());
  CHECK(ptr == &(result.value().get()));
}

TEST_CASE("ExtractByNameExtended - Edge cases - Float as int", "[valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(floatIntEdgeCase, manager);
  REQUIRE(parseResult);

  auto& fieldsOfRoot = parseResult.value().at(0).fields;
  int32_t* ptr = boost::get<int32_t>(&fieldsOfRoot.at(2).value);

  std::string invalidType;
  auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<int32_t>(
      "float", fieldsOfRoot, invalidType);
  REQUIRE(result.has_error());
}

TEST_CASE("ExtractByNameExtended - Edge cases - Float as float", "[valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(floatIntEdgeCase, manager);
  REQUIRE(parseResult);

  auto& fieldsOfRoot = parseResult.value().at(0).fields;
  vrml_proc::parser::model::float32_t* ptr = boost::get<vrml_proc::parser::model::float32_t>(&fieldsOfRoot.at(2).value);

  std::string invalidType;
  auto result =
      vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<vrml_proc::parser::model::float32_t>(
          "float", fieldsOfRoot, invalidType);
  REQUIRE(result.has_value());
  CHECK(ptr == &(result.value().get()));
}

TEST_CASE("ExtractByNameExtended - Edge cases - Complicated example", "[valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(complicatedEdgeCase, manager);
  REQUIRE(parseResult);

  auto& fieldsOfFirst = parseResult.value().at(1).fields;
  auto& nodeFirst =
      vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNode("children", fieldsOfFirst, manager)
          .value()
          .get();
  auto result1 = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByName<vrml_proc::parser::model::float32_t>(
      "float", nodeFirst.fields);
  REQUIRE(result1.has_value());

  auto& fieldsOfSecond = parseResult.value().at(2).fields;
  auto& nodeSecond =
      vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNode("children", fieldsOfSecond, manager)
          .value()
          .get();
  auto result2 = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByName<vrml_proc::parser::model::float32_t>(
      "float", nodeSecond.fields);
  REQUIRE(result2.has_value());

  CHECK(&(result1.value().get()) == &(result2.value().get()));

  auto result3 = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByName<int32_t>("int", nodeFirst.fields);
  REQUIRE(result3.has_value());

  auto result4 = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByName<int32_t>("int", nodeSecond.fields);
  REQUIRE(result4.has_value());

  CHECK(&(result3.value().get()) == &(result4.value().get()));

  auto& fieldsOfThird = parseResult.value().at(3).fields;
  auto result5 = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByName<vrml_proc::parser::model::float32_t>(
      "float", fieldsOfThird);
  auto result6 = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByName<int32_t>("int", fieldsOfThird);

  CHECK_FALSE(&(result5.value().get()) == &(result1.value().get()));
  CHECK_FALSE(&(result6.value().get()) == &(result3.value().get()));
}