#include <catch2/catch_test_macros.hpp>
#include <catch2/interfaces/catch_interfaces_capture.hpp>

#include <filesystem>

#include "test_data/VrmlFileTraversorTestDataset.hpp"
#include <Logger.hpp>

#include "../../test_utils/TestCommon.hpp"
#include "../../test_utils/TestTraversor.hpp"

#include <VrmlNodeManager.hpp>

TEST_CASE("Initialization") {
  vrml_proc::core::logger::InitLogging();

  std::filesystem::path executablePath = std::filesystem::current_path();
  std::filesystem::path filepath = executablePath / "testConfig.json";
  InitTesting(filepath);
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Box node", "[parsing][valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(validBoxNode, manager);
  REQUIRE(parseResult);

  CHECK(TraverseVrmlFileToMeshTask(parseResult, manager, 0));
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Box node empty", "[parsing][valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(validBoxNodeEmpty, manager);
  REQUIRE(parseResult);

  CHECK(TraverseVrmlFileToMeshTask(parseResult, manager, 0));
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - Box node - Wrong Field Type", "[parsing][invalid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(invalidBoxNodeWrongDataType, manager);
  REQUIRE(parseResult);

  CHECK_FALSE(TraverseVrmlFileToMeshTask(parseResult, manager));
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - Box node - Wrong Field Name", "[parsing][invalid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(invalidBoxNodeWrongFieldName, manager);
  REQUIRE(parseResult);

  CHECK_FALSE(TraverseVrmlFileToMeshTask(parseResult, manager));
  ;
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - WorldInfo node", "[parsing][valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(validWorldInfoNode, manager);
  REQUIRE(parseResult);

  CHECK(TraverseVrmlFileToMeshTask(parseResult, manager, 0));
}

TEST_CASE(
    "Parse VRML File - Invalid Input - Simple VRML File - WorldInfo node - Wrong Field Type", "[parsing][invalid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(invalidWorldInfoNodeWrongDataType, manager);
  REQUIRE(parseResult);

  CHECK_FALSE(TraverseVrmlFileToMeshTask(parseResult, manager));
}

TEST_CASE(
    "Parse VRML File - Invalid Input - Simple VRML File - WorldInfo node - Wrong Field Name", "[parsing][invalid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(invalidWorldInfoNodeWrongFieldName, manager);
  REQUIRE(parseResult);

  CHECK_FALSE(TraverseVrmlFileToMeshTask(parseResult, manager));
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - Group node - Unknown node", "[parsing][invalid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(invalidGroupUnknownNode, manager);
  REQUIRE(parseResult);

  CHECK_FALSE(TraverseVrmlFileToMeshTask(parseResult, manager));
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Group node", "[parsing][valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(validGroup, manager);
  REQUIRE(parseResult);

  CHECK(TraverseVrmlFileToMeshTask(parseResult, manager, 0));
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Shape node", "[parsing][valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(validShape, manager);
  REQUIRE(parseResult);

  CHECK(TraverseVrmlFileToMeshTask(parseResult, manager, 1));
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Shape node with Group merging", "[parsing][valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(validShapeMerge, manager);
  REQUIRE(parseResult);

  CHECK(TraverseVrmlFileToMeshTask(parseResult, manager, 3));
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Shape empty node", "[parsing][valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(validShapeEmpty, manager);
  REQUIRE(parseResult);

  CHECK(TraverseVrmlFileToMeshTask(parseResult, manager, 0));
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - Shape node", "[parsing][invalid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(invalidShapeWrongNodeForGeometryField, manager);
  REQUIRE(parseResult);

  CHECK_FALSE(TraverseVrmlFileToMeshTask(parseResult, manager));
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - IndexedFaceSet node I.", "[parsing][valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(validIndexedFaceSetNode, manager);
  REQUIRE(parseResult);

  CHECK(TraverseVrmlFileToMeshTask(parseResult, manager, 1));
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - IndexedFaceSet node II.", "[parsing][valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(validIndexedFaceSetNodeNotInShape, manager);
  REQUIRE(parseResult);

  CHECK(TraverseVrmlFileToMeshTask(parseResult, manager, 0));
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - IndexedFaceSet node III.", "[parsing][valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(validIndexedFaceSetPyramid, manager);
  REQUIRE(parseResult);

  CHECK(TraverseVrmlFileToMeshTask(parseResult, manager, 1));
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - IndexedFaceSet node I.", "[parsing][invalid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(invalidIndexedFaceSetNodeWrongFieldNodeHeader, manager);
  REQUIRE(parseResult);

  CHECK_FALSE(TraverseVrmlFileToMeshTask(parseResult, manager));
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Transformation node I.", "[parsing][valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(validTransformatioNode, manager);
  REQUIRE(parseResult);

  CHECK(TraverseVrmlFileToMeshTask(parseResult, manager, 1));
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Transformation node II.", "[parsing][valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(validTransformatioNodeEmpty, manager);
  REQUIRE(parseResult);

  CHECK(TraverseVrmlFileToMeshTask(parseResult, manager, 0));
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - Transformation node I.", "[parsing][invalid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(invalidTransformatioNodeWrongFieldNameCenter, manager);
  REQUIRE(parseResult);

  CHECK_FALSE(TraverseVrmlFileToMeshTask(parseResult, manager));
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - Transformation node II.", "[parsing][invalid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(invalidTransformatioNodeWrongFieldNameChildren, manager);
  REQUIRE(parseResult);

  CHECK_FALSE(TraverseVrmlFileToMeshTask(parseResult, manager));
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - Transformation node III.", "[parsing][invalid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(invalidTransformatioNodeWrongFieldTypeChildren, manager);
  REQUIRE(parseResult);

  CHECK_FALSE(TraverseVrmlFileToMeshTask(parseResult, manager));
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - Transformation node IV.", "[parsing][invalid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(invalidTransformatioNodeWrongFieldTypeScaleOrientation, manager);
  REQUIRE(parseResult);

  CHECK_FALSE(TraverseVrmlFileToMeshTask(parseResult, manager));
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Transformation node III.", "[parsing][valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(validTransformatioNodeNested, manager);
  REQUIRE(parseResult);

  CHECK(TraverseVrmlFileToMeshTask(parseResult, manager, 1));
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Switch node I.", "[parsing][valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(validSwitchNodeWithOneChoiceEmpty, manager);
  REQUIRE(parseResult);

  CHECK(TraverseVrmlFileToMeshTask(parseResult, manager, 0));
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Switch node II.", "[parsing][valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(validSwitchNodeWithMulipleChoicesEmpty, manager);
  REQUIRE(parseResult);

  CHECK(TraverseVrmlFileToMeshTask(parseResult, manager, 0));
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Switch node III.", "[parsing][valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(validSwitchNodeEmpty, manager);
  REQUIRE(parseResult);

  CHECK(TraverseVrmlFileToMeshTask(parseResult, manager, 0));
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Switch node IV.", "[parsing][valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(validSwitchNodeWithMulipleChoices, manager);
  REQUIRE(parseResult);

  CHECK(TraverseVrmlFileToMeshTask(parseResult, manager, 2));
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - Switch node I.", "[parsing][invalid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(invalidSwitchNodeWithWrongMulipleChoices, manager);
  REQUIRE(parseResult);

  CHECK_FALSE(TraverseVrmlFileToMeshTask(parseResult, manager));
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - Switch node II.", "[parsing][invalid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(invalidSwitchNodeInvalidFieldType, manager);
  REQUIRE(parseResult);

  CHECK_FALSE(TraverseVrmlFileToMeshTask(parseResult, manager));
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - Switch node III.", "[parsing][invalid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(invalidSwitchNodeWrongIndexUpper, manager);
  REQUIRE(parseResult);

  CHECK(TraverseVrmlFileToMeshTask(parseResult, manager, 0));
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - Switch node IV.", "[parsing][invalid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(invalidSwitchNodeWrongIndexLower, manager);
  REQUIRE(parseResult);

  CHECK(TraverseVrmlFileToMeshTask(parseResult, manager, 0));
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - Switch node V.", "[parsing][invalid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(invalidSwitchNodeEmptyChoice, manager);
  REQUIRE(parseResult);

  CHECK(TraverseVrmlFileToMeshTask(parseResult, manager, 0));
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Appearance node I.", "[parsing][valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(validAppearanceNodeEmpty, manager);
  REQUIRE(parseResult);

  CHECK(TraverseVrmlFileToMeshTask(parseResult, manager, 1));
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Appearance node II.", "[parsing][valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(validAppearanceNode, manager);
  REQUIRE(parseResult);

  CHECK(TraverseVrmlFileToMeshTask(parseResult, manager, 1));
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Appearance node III.", "[parsing][valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(validAppearanceNode2, manager);
  REQUIRE(parseResult);

  CHECK(TraverseVrmlFileToMeshTask(parseResult, manager, 1));
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - Appearance node I.", "[parsing][invalid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(invalidAppearanceNodeWrongMaterialFieldName, manager);
  REQUIRE(parseResult);

  CHECK_FALSE(TraverseVrmlFileToMeshTask(parseResult, manager));
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - Appearance node II.", "[parsing][invalid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(invalidAppearanceNodeWrongMaterialNodeHeader, manager);
  REQUIRE(parseResult);

  CHECK_FALSE(TraverseVrmlFileToMeshTask(parseResult, manager));
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - Appearance node III.", "[parsing][invalid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(invalidAppearanceNodeWrongDataType, manager);
  REQUIRE(parseResult);

  CHECK_FALSE(TraverseVrmlFileToMeshTask(parseResult, manager));
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - IndexedLineSet node I.", "[parsing][valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(validIndexedLineSetNode, manager);
  REQUIRE(parseResult);

  CHECK(TraverseVrmlFileToMeshTask(parseResult, manager, 1));
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - IndexedLineSet node I.", "[parsing][invalid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(invalidIndexedLineSetNodeWrongUnknownField, manager);
  REQUIRE(parseResult);

  CHECK_FALSE(TraverseVrmlFileToMeshTask(parseResult, manager));
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - IndexedLineSet node II.", "[parsing][invalid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(invalidIndexedLineSetNodeWrongFieldType, manager);
  REQUIRE(parseResult);

  CHECK_FALSE(TraverseVrmlFileToMeshTask(parseResult, manager));
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Anchor node", "[parsing][valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(validAnchor, manager);
  REQUIRE(parseResult);

  CHECK(TraverseVrmlFileToMeshTask(parseResult, manager, 0));
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Billboard node", "[parsing][valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(validBillboard, manager);
  REQUIRE(parseResult);

  CHECK(TraverseVrmlFileToMeshTask(parseResult, manager, 0));
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Collision node", "[parsing][valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(validCollision, manager);
  REQUIRE(parseResult);

  CHECK(TraverseVrmlFileToMeshTask(parseResult, manager, 0));
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - Inline node", "[parsing][valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(validInline, manager);
  REQUIRE(parseResult);

  CHECK(TraverseVrmlFileToMeshTask(parseResult, manager, 0));
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - LOD node", "[parsing][valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(validLOD, manager);
  REQUIRE(parseResult);

  CHECK(TraverseVrmlFileToMeshTask(parseResult, manager, 0));
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - FontStyle node I.", "[parsing][valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(validFontStyle1, manager);
  REQUIRE(parseResult);

  CHECK(TraverseVrmlFileToMeshTask(parseResult, manager, 0));
}

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File - FontStyle node II.", "[parsing][valid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(validFontStyle2, manager);
  REQUIRE(parseResult);

  CHECK(TraverseVrmlFileToMeshTask(parseResult, manager, 0));
}

TEST_CASE("Parse VRML File - Invalid Input - Simple VRML File - FontStyle node", "[parsing][invalid]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(invalidFontStyle, manager);
  REQUIRE(parseResult);

  CHECK_FALSE(TraverseVrmlFileToMeshTask(parseResult, manager, 0));
}

TEST_CASE("Parse VRMLFile From File - Valid Input - Actin", "[parsing][valid][fromfile]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(
      std::filesystem::path(ReadTestInfo().baseInputPath) / std::filesystem::path(ReadTestInfo().testFiles.at("ACTIN")),
      manager);
  REQUIRE(parseResult);

  auto config = std::make_shared<to_geom::core::config::ToGeomConfig>();

  GENERATE_TEST_OUTPUT_FILENAME(filepath);
  CHECK(TraverseVrmlFileToMeshTask(
      parseResult, manager, 5, std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath, 5, config));
  CHECK(HaveSimiliarSizes(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath,
      std::filesystem::path(ReadTestInfo().baseExpectedOutputPath) / filepath, 0));
}

TEST_CASE("Parse VRMLFile From File - Valid Input - Nucleus", "[parsing][valid][fromfile]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(std::filesystem::path(ReadTestInfo().baseInputPath) /
                                       std::filesystem::path(ReadTestInfo().testFiles.at("NUCLEUS")),
      manager);
  REQUIRE(parseResult);

  auto config = std::make_shared<to_geom::core::config::ToGeomConfig>();

  GENERATE_TEST_OUTPUT_FILENAME(filepath);
  CHECK(TraverseVrmlFileToMeshTask(
      parseResult, manager, 2, std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath, 2, config));
  CHECK(HaveSimiliarSizes(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath,
      std::filesystem::path(ReadTestInfo().baseExpectedOutputPath) / filepath, 0));
}

TEST_CASE("Parse VRMLFile From File - Valid Input - Tubulin", "[parsing][valid][fromfile]") {
  vrml_proc::parser::service::VrmlNodeManager manager;
  auto parseResult = ParseVrmlFile(std::filesystem::path(ReadTestInfo().baseInputPath) /
                                       std::filesystem::path(ReadTestInfo().testFiles.at("TUBULIN")),
      manager);
  REQUIRE(parseResult);

  auto config = std::make_shared<to_geom::core::config::ToGeomConfig>();

  GENERATE_TEST_OUTPUT_FILENAME(filepath);
  CHECK(TraverseVrmlFileToMeshTask(
      parseResult, manager, 399, std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath, 399, config));
  CHECK(HaveSimiliarSizes(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath,
      std::filesystem::path(ReadTestInfo().baseExpectedOutputPath) / filepath, 0));
}

// TEST_CASE("Parse VRMLFile From File - Valid Input - Tile 3x5 - 5 Stitch", "[parsing][valid][fromfile]") {
//
//     vrml_proc::parser::service::VrmlNodeManager manager;
//     auto parseResult = ParseVrmlFile(std::filesystem::path(ReadTestInfo().baseInputPath) /
//     std::filesystem::path(ReadTestInfo().testFiles.at("TILES")), manager); REQUIRE(parseResult);
//
//     vrml_proc::core::config::VrmlProcConfig config;
//
//     GENERATE_TEST_OUTPUT_FILENAME(filepath);
//     CHECK(TraverseVrmlFileToMeshTask(parseResult, manager, 64611,
//     std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath, 64611, config));
//     CHECK(HaveSimiliarSizes(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath,
//     std::filesystem::path(ReadTestInfo().baseExpectedOutputPath) / filepath, 0));
// }