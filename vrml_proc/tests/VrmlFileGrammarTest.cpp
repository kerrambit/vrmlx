#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <vector> 

#include <boost/optional/optional.hpp>
#include <boost/variant/detail/apply_visitor_unary.hpp>
#include <boost/variant/get.hpp>
#include <boost/variant/recursive_wrapper.hpp>
#include <boost/variant/static_visitor.hpp>
#include <boost/variant/variant.hpp>

#include "test_data/VrmlFileGrammarTestDataset.hpp"
#include <Int32Array.hpp>
#include <Logger.hpp>
#include <ParserResult.hpp>
#include <UseNode.hpp>
#include <Vec3f.hpp>
#include <Vec3fArray.hpp>
#include <VrmlField.hpp>
#include <VrmlFile.hpp>
#include <VrmlNode.hpp>
#include <VrmlNodeManager.hpp>
#include <VrmlParser.hpp>
#include <VrmlUnits.hpp>

#include "../../test_utils/TestCommon.hpp"

TEST_CASE("Initialization") { vrml_proc::core::logger::InitLogging(); }

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File", "[parsing][valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(simpleValid, manager);
  REQUIRE(parseResult);

  vrml_proc::parser::model::VrmlNode root = parseResult.value().at(0);
  CHECK(root.header == "WorldInfo");

  {
    auto& field = root.fields.at(0);
    CHECK(field.name == "title");

    REQUIRE(boost::get<std::string>(&field.value) != nullptr);
    auto& value = boost::get<std::string>(field.value);
    CHECK(value == "cube.wrl");
  }

  {
    auto& field = root.fields.at(1);
    CHECK(field.name == "info");

    REQUIRE(boost::get<std::string>(&field.value) != nullptr);
    auto& value = boost::get<std::string>(field.value);
    CHECK(value == "VRML file.");
  }

  CHECK(manager.GetDefNodesTotalCount() == 0);
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File Ivalid", "[parsing][invalid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(simpleInvalidChar, manager);
  REQUIRE_FALSE(parseResult);
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File Ivalid Starting With Number", "[parsing][invalid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(simpleInvalidStartingWithNumber, manager);
  REQUIRE_FALSE(parseResult);
}

TEST_CASE("Parse VRML File - Valid Input - Two Simple Nodes", "[parsing][valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(twoSimpleNodes, manager);
  REQUIRE(parseResult);

  {
    vrml_proc::parser::model::VrmlNode firstNode = parseResult.value().at(0);
    CHECK(firstNode.header == "WorldInfo");

    {
      auto& field = firstNode.fields.at(0);
      CHECK(field.name == "title");

      REQUIRE(boost::get<std::string>(&field.value) != nullptr);
      auto& value = boost::get<std::string>(field.value);
      CHECK(value == "cube.wrl");
    }

    {
      auto& field = firstNode.fields.at(1);
      CHECK(field.name == "info");

      REQUIRE(boost::get<std::string>(&field.value) != nullptr);
      auto& value = boost::get<std::string>(field.value);
      CHECK(value == "VRML file.");
    }
  }

  {
    vrml_proc::parser::model::VrmlNode secondNode = parseResult.value().at(1);
    CHECK(secondNode.header == "Group");

    {
      auto& field = secondNode.fields.at(0);
      CHECK(field.name == "geometry");

      auto* value = boost::get<vrml_proc::parser::model::Vec3fArray>(&field.value);
      REQUIRE(value != nullptr);
      CHECK_THAT(value->vectors.at(0).x, Catch::Matchers::WithinAbs(58.5, 0.1));
      CHECK_THAT(value->vectors.at(0).y, Catch::Matchers::WithinAbs(458.02, 0.1));
      CHECK_THAT(value->vectors.at(0).z, Catch::Matchers::WithinAbs(68.78, 0.1));
    }

    {
      auto& field = secondNode.fields.at(1);
      CHECK(field.name == "geometry");

      auto* value = boost::get<vrml_proc::parser::model::Vec3f>(&field.value);
      REQUIRE(value != nullptr);
      CHECK_THAT(value->x, Catch::Matchers::WithinAbs(100.001001, 0.1));
      CHECK_THAT(value->y, Catch::Matchers::WithinAbs(5.55317, 0.1));
      CHECK_THAT(value->z, Catch::Matchers::WithinAbs(-3.05561, 0.1));
    }
  }

  CHECK(manager.GetDefNodesTotalCount() == 0);
}

TEST_CASE("Parse VRML File - Valid Input - Node In Node", "[parsing][valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(nodeInNode, manager);
  REQUIRE(parseResult);

  {
    vrml_proc::parser::model::VrmlNode root = parseResult.value().at(0);
    CHECK(root.header == "Shape");

    auto& field = root.fields.at(0);
    CHECK(field.name == "appearance");

    auto* value = boost::get<vrml_proc::parser::model::VrmlNode>(&field.value);
    REQUIRE(value != nullptr);

    CHECK(value->header == "Appearence");

    {
      auto& field = value->fields.at(0);
      CHECK(field.name == "ambientIntensity");

      REQUIRE(boost::get<vrml_proc::parser::model::float32_t>(&field.value) != nullptr);
      auto value = boost::get<vrml_proc::parser::model::float32_t>(field.value);
      CHECK_THAT(value, Catch::Matchers::WithinAbs(0.02, 0.1));
    }
  }

  CHECK(manager.GetDefNodesTotalCount() == 0);
}

TEST_CASE("Parse VRML File - Valid Input - Quite Deep Recursive Nodes", "[parsing][valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(quiteDeepRecursiveNode, manager);
  REQUIRE(parseResult);

  {
    vrml_proc::parser::model::VrmlNode root = parseResult.value().at(0);
    CHECK(root.header == "Shape");

    auto& field = root.fields.at(0);
    CHECK(field.name == "appearance");

    auto* value = boost::get<vrml_proc::parser::model::VrmlNode>(&field.value);
    REQUIRE(value != nullptr);

    CHECK(value->header == "Appearence");
    {
      auto& field = value->fields.at(0);
      CHECK(field.name == "ambientIntensity");

      REQUIRE(boost::get<vrml_proc::parser::model::float32_t>(&field.value) != nullptr);
      auto ambientIntensity = boost::get<vrml_proc::parser::model::float32_t>(field.value);
      CHECK_THAT(ambientIntensity, Catch::Matchers::WithinAbs(0.02, 0.1));

      {
        auto& field = value->fields.at(1);
        CHECK(field.name == "appearance");

        auto* value = boost::get<vrml_proc::parser::model::VrmlNode>(&field.value);
        REQUIRE(value != nullptr);
        CHECK(value->header == "Appearence");

        auto& innerField = value->fields.at(0);
        CHECK(innerField.name == "lightExposure");

        REQUIRE(boost::get<int32_t>(&innerField.value) != nullptr);
        auto lightExposure = boost::get<int32_t>(innerField.value);
        CHECK(lightExposure == 42);
      }
    }
  }

  CHECK(manager.GetDefNodesTotalCount() == 0);
}

TEST_CASE("Parse VRMLFile - Valid Input - Group With Nodes Array", "[parsing][valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(groupWithNodesArray, manager);
  REQUIRE(parseResult);

  {
    vrml_proc::parser::model::VrmlNode root = parseResult.value().at(0);
    CHECK(root.header == "Group");

    auto& field = root.fields.at(0);
    CHECK(field.name == "children");

    auto* value =
        boost::get<std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::model::VrmlNode>,
                                              boost::recursive_wrapper<vrml_proc::parser::model::UseNode>>>>(&field.value);
    REQUIRE(value != nullptr);

    {
      auto& firstVariant = value->at(0);

      struct VrmlNodeVisitor : public boost::static_visitor<void> {
        void operator()(vrml_proc::parser::model::VrmlNode& vrmlNode) const {
          REQUIRE(vrmlNode.header == "Group");
          auto& field = vrmlNode.fields.at(0);
          CHECK(field.name == "bboxCenter");

          REQUIRE(boost::get<vrml_proc::parser::model::Vec3f>(&field.value) != nullptr);
          auto* bboxCenter = boost::get<vrml_proc::parser::model::Vec3f>(&field.value);

          CHECK_THAT(bboxCenter->x, Catch::Matchers::WithinAbs(0.058, 0.01));
          CHECK_THAT(bboxCenter->y, Catch::Matchers::WithinAbs(0.0, 0.0));
          CHECK_THAT(bboxCenter->z, Catch::Matchers::WithinAbs(15.0, 0.0));
        }

        void operator()(vrml_proc::parser::model::UseNode& useNode) const { FAIL("Unexpected type inside the variant."); }
      };

      VrmlNodeVisitor visitor;
      boost::apply_visitor(visitor, firstVariant);
    }

    {
      auto& secondVariant = value->at(1);

      struct SecondVrmlNodeVisitor : public boost::static_visitor<void> {
        void operator()(vrml_proc::parser::model::VrmlNode& vrmlNode) const {
          REQUIRE(vrmlNode.header == "Group");
          auto&& field = vrmlNode.fields.at(0);
          CHECK(field.name == "bboxSize");

          REQUIRE(boost::get<vrml_proc::parser::model::Vec2f>(&field.value) != nullptr);
          auto* bboxCenter = boost::get<vrml_proc::parser::model::Vec2f>(&field.value);

          CHECK_THAT(bboxCenter->u, Catch::Matchers::WithinAbs(-1.0, 0.0));
          CHECK_THAT(bboxCenter->v, Catch::Matchers::WithinAbs(-1.0, 0.0));
        }

        void operator()(vrml_proc::parser::model::UseNode& useNode) const { FAIL("Unexpected type inside the variant."); }
      };

      SecondVrmlNodeVisitor visitor;
      boost::apply_visitor(visitor, secondVariant);
    }
  }

  CHECK(manager.GetDefNodesTotalCount() == 0);
}

TEST_CASE("Parse VRML File - Valid Input - Simple DEF Node", "[parsing][valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(simpleDefNode, manager);
  REQUIRE(parseResult);

  auto& root = parseResult.value().at(0);
  auto& field = root.fields.at(0);

  CHECK(field.name == "children");

  auto* value =
      boost::get<std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::model::VrmlNode>,
                                            boost::recursive_wrapper<vrml_proc::parser::model::UseNode>>>>(&field.value);
  REQUIRE(value != nullptr);

  uintptr_t variantPtr;
  {
    auto& variant = value->at(0);

    struct VrmlNodeVisitor : public boost::static_visitor<void> {
      void operator()(vrml_proc::parser::model::VrmlNode& vrmlNode) {
        REQUIRE(vrmlNode.header == "Group");
        ptr = reinterpret_cast<uintptr_t>(&vrmlNode);

        REQUIRE(vrmlNode.definitionName.has_value());
        CHECK(vrmlNode.definitionName.value() == "id");

        auto& field = vrmlNode.fields.at(0);
        CHECK(field.name == "bboxCenter");

        REQUIRE(boost::get<vrml_proc::parser::model::Vec3f>(&field.value) != nullptr);
        auto* bboxCenter = boost::get<vrml_proc::parser::model::Vec3f>(&field.value);

        CHECK_THAT(bboxCenter->x, Catch::Matchers::WithinAbs(0.0587, 0.01));
        CHECK_THAT(bboxCenter->y, Catch::Matchers::WithinAbs(0.0, 0.0));
        CHECK_THAT(bboxCenter->z, Catch::Matchers::WithinAbs(15.0, 0.0));
      }

      void operator()(vrml_proc::parser::model::UseNode& useNode) { FAIL("Unexpected type inside the variant."); }

      uintptr_t ptr;
    };

    VrmlNodeVisitor visitor;
    boost::apply_visitor(visitor, variant);
    variantPtr = visitor.ptr;
  }

  auto def = manager.GetDefinitionNode("id");
  REQUIRE(def.has_value());
  CHECK(reinterpret_cast<uintptr_t>(&def.value().get()) == variantPtr);
}

TEST_CASE("Parse VRML File - Valid Input - Simple USE Node", "[parsing][valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(simpleUseNode, manager);
  REQUIRE(parseResult);

  {
    vrml_proc::parser::model::VrmlNode root = parseResult.value().at(0);
    CHECK(root.header == "Group");

    auto& field = root.fields.at(0);
    CHECK(field.name == "children");

    auto* value =
        boost::get<std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::model::VrmlNode>,
                                              boost::recursive_wrapper<vrml_proc::parser::model::UseNode>>>>(&field.value);
    REQUIRE(value != nullptr);

    {
      auto& variant = value->at(0);

      struct VrmlNodeVisitor : public boost::static_visitor<void> {
        void operator()(vrml_proc::parser::model::VrmlNode& vrmlNode) const {
          REQUIRE(vrmlNode.header == "Group");
          auto& field = vrmlNode.fields.at(0);

          REQUIRE(vrmlNode.definitionName.has_value());
          CHECK(vrmlNode.definitionName.value() == "id");

          CHECK(field.name == "bboxCenter");

          REQUIRE(boost::get<vrml_proc::parser::model::Vec2fArray>(&field.value) != nullptr);
          auto* bboxCenter = boost::get<vrml_proc::parser::model::Vec2fArray>(&field.value);

          CHECK_THAT(bboxCenter->vectors.at(0).u, Catch::Matchers::WithinAbs(0.058, 0.01));
          CHECK_THAT(bboxCenter->vectors.at(0).v, Catch::Matchers::WithinAbs(0.0, 0.0));
          CHECK_THAT(bboxCenter->vectors.at(1).u, Catch::Matchers::WithinAbs(15.0, 0.0));
          CHECK_THAT(bboxCenter->vectors.at(1).v, Catch::Matchers::WithinAbs(69.0, 0.0));
        }

        void operator()(vrml_proc::parser::model::UseNode& useNode) const { FAIL("Unexpected type inside the variant."); }
      };

      VrmlNodeVisitor visitor;
      boost::apply_visitor(visitor, variant);
    }
  }

  {
    vrml_proc::parser::model::VrmlNode root = parseResult.value().at(1);
    CHECK(root.header == "Group");

    auto& field = root.fields.at(0);
    CHECK(field.name == "instance");

    auto* value = boost::get<vrml_proc::parser::model::UseNode>(&field.value);
    REQUIRE(value != nullptr);

    CHECK(value->identifier == "id");
  }

  auto def = manager.GetDefinitionNode("id");
  REQUIRE(def.has_value());
}

TEST_CASE("Parse VRM LFile - Valid Input - Node With Switch", "[parsing][valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(nodeWithSwitch, manager);
  REQUIRE(parseResult);

  vrml_proc::parser::model::VrmlNode root = parseResult.value().at(0);
  CHECK(root.header == "Group");

  auto& field = root.fields.at(0);
  CHECK(field.name == "children");

  auto* children =
      boost::get<std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::model::VrmlNode>,
                                            boost::recursive_wrapper<vrml_proc::parser::model::UseNode>>>>(&field.value);
  REQUIRE(children != nullptr);

  {
    auto& variant = children->at(0);

    struct VrmlNodeVisitor : public boost::static_visitor<void> {
      void operator()(vrml_proc::parser::model::VrmlNode& vrmlNode) const {
        REQUIRE(vrmlNode.definitionName.has_value());
        CHECK(vrmlNode.definitionName.value() == "A1");
        CHECK(vrmlNode.header == "Switch");

        {
          auto& field = vrmlNode.fields.at(0);
          CHECK(field.name == "whichChoice");

          REQUIRE(boost::get<int32_t>(&field.value) != nullptr);
          auto whichChoice = boost::get<int32_t>(field.value);
          CHECK(whichChoice == 0);
        }

        {
          auto& field = vrmlNode.fields.at(1);
          CHECK(field.name == "choice");

          REQUIRE(boost::get<vrml_proc::parser::model::VrmlNode>(&field.value) != nullptr);
          auto* child = boost::get<vrml_proc::parser::model::VrmlNode>(&field.value);

          CHECK(child->header == "Empty");
          REQUIRE(child->definitionName.has_value());
          CHECK(child->definitionName.value() == "C2");
          CHECK(child->fields.empty());
        }
      }

      void operator()(vrml_proc::parser::model::UseNode& useNode) const { FAIL("Unexpected type inside the variant."); }
    };

    VrmlNodeVisitor visitor;
    boost::apply_visitor(visitor, variant);
  }

  {
    auto def = manager.GetDefinitionNode("A1");
    REQUIRE(def.has_value());
  }

  {
    auto def = manager.GetDefinitionNode("C2");
    REQUIRE(def.has_value());
  }
}

TEST_CASE("Parse VRMLFile - Valid Input - Node With Boolean", "[parsing][valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(nodeWithBoolean, manager);
  REQUIRE(parseResult);

  vrml_proc::parser::model::VrmlNode root = parseResult.value().at(0);
  CHECK(root.header == "Group");

  auto& field = root.fields.at(0);
  CHECK(field.name == "shape");

  auto* shape = boost::get<vrml_proc::parser::model::VrmlNode>(&field.value);
  REQUIRE(shape != nullptr);

  CHECK(shape->header == "Shape");
  CHECK(((!shape->definitionName.has_value()) || shape->definitionName.value().empty()));
  CHECK(shape->fields.size() == 1);
  CHECK(shape->fields.at(0).name == "geometry");

  auto* indexedFaceSet = boost::get<vrml_proc::parser::model::VrmlNode>(&shape->fields.at(0).value);
  REQUIRE(indexedFaceSet != nullptr);

  CHECK(indexedFaceSet->header == "IndexedFaceSet");
  CHECK(((!indexedFaceSet->definitionName.has_value()) || indexedFaceSet->definitionName.value().empty()));
  CHECK(indexedFaceSet->fields.size() == 3);

  {
    auto& child = indexedFaceSet->fields.at(0);
    CHECK(child.name == "coordIndex");

    auto* data = boost::get<vrml_proc::parser::model::Int32Array>(&child.value);
    REQUIRE(data != nullptr);

    std::vector<int> expectedValues = {944, 942, 943,    -1,     947,    945, 946,    -1,     947,    949, 948,
                                       -1,  945, 947,    948,    -1,     946, 942,    944,    -1,     946, 944,
                                       947, -1,  108889, 108856, 182463, -1,  108863, 108433, 182449, -1};

    REQUIRE(data->integers.size() == expectedValues.size());

    for (size_t i = 0; i < expectedValues.size(); ++i) {
      CHECK(data->integers.at(i) == expectedValues.at(i));
    }
  }

  {
    auto& child = indexedFaceSet->fields.at(1);
    CHECK(child.name == "ccw");

    auto data = boost::get<bool>(child.value);
    CHECK(data);
  }

  {
    auto& child = indexedFaceSet->fields.at(2);
    CHECK(child.name == "solid");

    auto data = boost::get<bool>(child.value);
    CHECK_FALSE(data);
  }
}

TEST_CASE("Parse VRMLFile - Valid Input - Node With UTF8", "[parsing][valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(nodeWithUtf8, manager);
  REQUIRE(parseResult);

  vrml_proc::parser::model::VrmlNode root = parseResult.value().at(0);
  CHECK(root.header == "ČeskáSkupina");

  auto& field = root.fields.at(0);
  CHECK(field.name == "políčko");

  auto* shape = boost::get<vrml_proc::parser::model::VrmlNode>(&field.value);
  REQUIRE(shape != nullptr);

  CHECK(shape->header == "Tvar");
  CHECK(((!shape->definitionName.has_value()) || shape->definitionName.value().empty()));
  CHECK(shape->fields.size() == 1);
  CHECK(shape->fields.at(0).name == "ß");

  auto* indexedFaceSet = boost::get<vrml_proc::parser::model::VrmlNode>(&shape->fields.at(0).value);
  REQUIRE(indexedFaceSet != nullptr);

  CHECK(indexedFaceSet->header == "开儿艾诶开伊艾艾西吉艾艾伊娜伊");
  CHECK(((!indexedFaceSet->definitionName.has_value()) || indexedFaceSet->definitionName.value().empty()));
  CHECK(indexedFaceSet->fields.size() == 1);

  {
    auto& child = indexedFaceSet->fields.at(0);
    CHECK(child.name == "koordináty");

    auto* data = boost::get<vrml_proc::parser::model::Int32Array>(&child.value);
    REQUIRE(data != nullptr);

    std::vector<int> expectedValues = {944, 942, 943,    -1,     947,    945, 946,    -1,     947,    949, 948,
                                       -1,  945, 947,    948,    -1,     946, 942,    944,    -1,     946, 944,
                                       947, -1,  108889, 108856, 182463, -1,  108863, 108433, 182449, -1};

    REQUIRE(data->integers.size() == expectedValues.size());

    for (size_t i = 0; i < expectedValues.size(); ++i) {
      CHECK(data->integers.at(i) == expectedValues.at(i));
    }
  }
}

TEST_CASE("Parse VRML File - Valid Input - Empty node", "[parsing][valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(emptyNode, manager);
  REQUIRE(parseResult);

  const auto& root = parseResult.value().at(0);
  CHECK(root.header == "Group");

  REQUIRE(root.fields.size() == 1);
  const auto& subgroupField = root.fields.at(0);
  CHECK(subgroupField.name == "subgroup");

  const auto* subgroup = boost::get<vrml_proc::parser::model::VrmlNode>(&subgroupField.value);
  REQUIRE(subgroup != nullptr);
  CHECK(subgroup->header == "Group");

  REQUIRE(subgroup->fields.size() == 1);
  const auto& subsubgroupField = subgroup->fields.at(0);
  CHECK(subsubgroupField.name == "subsubgroup");

  const auto* subsubgroup = boost::get<vrml_proc::parser::model::VrmlNode>(&subsubgroupField.value);
  REQUIRE(subsubgroup != nullptr);
  CHECK(subsubgroup->header == "Group");

  REQUIRE(subsubgroup->fields.empty());
}

TEST_CASE("Parse VRML File - Valid Input - Complicated node", "[parsing][valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(complicatedNode, manager);
  REQUIRE(parseResult);
}

TEST_CASE("Parse VRML File - Valid Input - Complicated node with complicated DEF names", "[parsing][valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(complicatedNodeWithComplicatedDefNodeNames, manager);
  REQUIRE(parseResult);
}
