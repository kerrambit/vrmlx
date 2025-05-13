#include <catch2/catch_test_macros.hpp>
#include <catch2/interfaces/catch_interfaces_capture.hpp>

#include <algorithm>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <BoxCalculator.hpp>
#include <CalculatorError.hpp>
#include <IndexedFaceSetCalculator.hpp>
#include <Int32Array.hpp>
#include <Logger.hpp>
#include <ModelValidationError.hpp>
#include <StlFileWriter.hpp>
#include <Transformation.hpp>
#include <TransformationMatrix.hpp>
#include <Vec3f.hpp>
#include <VrmlUnits.hpp>
#include <AlphaShapeCalculator.hpp>

#include "../../test_utils/TestCommon.hpp"

TEST_CASE("Initialization") {
  vrml_proc::core::logger::InitLogging();

  std::filesystem::path executablePath = std::filesystem::current_path();
  std::filesystem::path filepath = executablePath / "testConfig.json";
  InitTesting(filepath);
}

TEST_CASE("BoxCalculator - Transform tests", "[valid]") {
  using vrml_proc::math::Transformation;
  using vrml_proc::math::TransformationMatrix;
  using vrml_proc::math::UpdateTransformationMatrix;
  using vrml_proc::parser::model::Vec3f;
  using vrml_proc::parser::model::Vec4f;

  Vec3f size;
  size.x = 10.0f;
  size.y = 20.0f;
  size.z = 30.0f;

  to_geom::calculator::BoxCalculator calculator = to_geom::calculator::BoxCalculator();
  to_geom::core::io::StlFileWriter writer;
  std::string testName = Catch::getResultCapture().getCurrentTestName();
  std::replace(testName.begin(), testName.end(), ' ', '_');

  // --- ///

  TransformationMatrix matrix;
  auto result0 = (calculator.Generate3DMesh({std::cref(size)}, matrix)).value();
  {
    GENERATE_TEST_OUTPUT_FILENAME_EXTRA_NAME(filepath, "noTransform");
    writer.Write(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath, *(result0));
    CHECK(AreBinaryFilesEqual(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath,
        std::filesystem::path(ReadTestInfo().baseExpectedOutputPath) / filepath));
  }

  // --- //

  Transformation transformationData;
  matrix = UpdateTransformationMatrix(matrix, transformationData);
  auto result1 = (calculator.Generate3DMesh({std::cref(size)}, matrix)).value();
  {
    GENERATE_TEST_OUTPUT_FILENAME_EXTRA_NAME(filepath, "defaultTransform");
    writer.Write(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath, *(result1));
    CHECK(AreBinaryFilesEqual(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath,
        std::filesystem::path(ReadTestInfo().baseExpectedOutputPath) / filepath));
  }

  // --- //

  transformationData = Transformation();
  matrix = TransformationMatrix();
  transformationData.rotation = Vec4f(0.0f, 0.0f, 1.0f, 0.785398163f);
  matrix = UpdateTransformationMatrix(matrix, transformationData);
  auto result2 = (calculator.Generate3DMesh({std::cref(size)}, matrix)).value();
  {
    GENERATE_TEST_OUTPUT_FILENAME_EXTRA_NAME(filepath, "zAxisRotation");
    writer.Write(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath, *(result2));
    CHECK(AreBinaryFilesEqual(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath,
        std::filesystem::path(ReadTestInfo().baseExpectedOutputPath) / filepath));
  }

  // --- //

  transformationData = Transformation();
  matrix = TransformationMatrix();
  transformationData.center = vrml_proc::parser::model::Vec3f(10.0f, 10.0f, 0.0f);
  transformationData.rotation = Vec4f(0.0f, 0.0f, 1.0f, 0.785398163f);
  matrix = UpdateTransformationMatrix(matrix, transformationData);
  auto result3 = (calculator.Generate3DMesh({std::cref(size)}, matrix)).value();
  {
    GENERATE_TEST_OUTPUT_FILENAME_EXTRA_NAME(filepath, "zAxisRotationWithCenter");
    writer.Write(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath, *(result3));
    CHECK(AreBinaryFilesEqual(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath,
        std::filesystem::path(ReadTestInfo().baseExpectedOutputPath) / filepath));
  }

  // --- //

  transformationData = Transformation();
  matrix = TransformationMatrix();
  transformationData.center = vrml_proc::parser::model::Vec3f(10.0f, 10.0f, 0.0f);
  matrix = UpdateTransformationMatrix(matrix, transformationData);
  auto result4 = (calculator.Generate3DMesh({std::cref(size)}, matrix)).value();
  {
    GENERATE_TEST_OUTPUT_FILENAME_EXTRA_NAME(filepath, "center");
    writer.Write(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath, *(result4));
    CHECK(AreBinaryFilesEqual(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath,
        std::filesystem::path(ReadTestInfo().baseExpectedOutputPath) / filepath));
  }

  // --- //

  transformationData = Transformation();
  matrix = TransformationMatrix();
  transformationData.rotation = Vec4f(0.0f, 1.0f, 0.0f, 0.785398163f);
  matrix = UpdateTransformationMatrix(matrix, transformationData);
  auto result5 = (calculator.Generate3DMesh({std::cref(size)}, matrix)).value();
  {
    GENERATE_TEST_OUTPUT_FILENAME_EXTRA_NAME(filepath, "yAsixRotation");
    writer.Write(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath, *(result5));
    CHECK(AreBinaryFilesEqual(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath,
        std::filesystem::path(ReadTestInfo().baseExpectedOutputPath) / filepath));
  }

  // --- //

  transformationData = Transformation();
  matrix = TransformationMatrix();
  transformationData.center = vrml_proc::parser::model::Vec3f(10.0f, 10.0f, 10.0f);
  transformationData.rotation = Vec4f(0.0f, 1.0f, 0.0f, 0.785398163f);
  matrix = UpdateTransformationMatrix(matrix, transformationData);
  auto result6 = (calculator.Generate3DMesh({std::cref(size)}, matrix)).value();
  {
    GENERATE_TEST_OUTPUT_FILENAME_EXTRA_NAME(filepath, "yAsixRotationWithCenter");
    writer.Write(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath, *(result6));
    CHECK(AreBinaryFilesEqual(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath,
        std::filesystem::path(ReadTestInfo().baseExpectedOutputPath) / filepath));
  }

  // --- //

  transformationData = Transformation();
  matrix = TransformationMatrix();
  transformationData.translation = vrml_proc::parser::model::Vec3f(10.0f, 10.0f, 10.0f);
  transformationData.rotation = Vec4f(0.0f, 0.0f, 1.0f, 0.785398163f);
  matrix = UpdateTransformationMatrix(matrix, transformationData);
  auto result7 = (calculator.Generate3DMesh({std::cref(size)}, matrix)).value();
  {
    GENERATE_TEST_OUTPUT_FILENAME_EXTRA_NAME(filepath, "zAsixRotationWithTranslation");
    writer.Write(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath, *(result7));
    CHECK(AreBinaryFilesEqual(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath,
        std::filesystem::path(ReadTestInfo().baseExpectedOutputPath) / filepath));
  }

  // --- //

  transformationData = Transformation();
  matrix = TransformationMatrix();
  transformationData.translation = vrml_proc::parser::model::Vec3f(0.0f, -25.0f, 0.0f);
  transformationData.rotation = vrml_proc::parser::model::Vec4f(0.0f, 0.0f, 1.0f, 0.785398163f);
  transformationData.scale = vrml_proc::parser::model::Vec3f(2.0f, 1.0f, 1.0f);
  transformationData.scaleOrientation = vrml_proc::parser::model::Vec4f(
      0.0f, 0.0f, 1.0f, 0.785398163f);  // without it applies scaling to world coordinates
  matrix = vrml_proc::math::UpdateTransformationMatrix(matrix, transformationData);
  auto result8 = (calculator.Generate3DMesh({std::cref(size)}, matrix)).value();
  {
    GENERATE_TEST_OUTPUT_FILENAME_EXTRA_NAME(filepath, "zAsixRotationWithTranslationAndScale");
    writer.Write(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath, *(result8));
    CHECK(AreBinaryFilesEqual(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath,
        std::filesystem::path(ReadTestInfo().baseExpectedOutputPath) / filepath));
  }

  // --- //

  transformationData = Transformation();
  matrix = TransformationMatrix();
  transformationData.translation = vrml_proc::parser::model::Vec3f(0.0f, -25.0f, 0.0f);
  transformationData.rotation = vrml_proc::parser::model::Vec4f(0.0f, 0.0f, 1.0f, 0.785398163f);
  transformationData.scale = vrml_proc::parser::model::Vec3f(2.0f, 1.0f, 1.0f);
  matrix = vrml_proc::math::UpdateTransformationMatrix(matrix, transformationData);
  auto result9 = (calculator.Generate3DMesh({std::cref(size)}, matrix)).value();
  {
    GENERATE_TEST_OUTPUT_FILENAME_EXTRA_NAME(filepath, "zAsixRotationWithTranslationAndScaleInWorldCoords");
    writer.Write(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath, *(result9));
    CHECK(AreBinaryFilesEqual(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath,
        std::filesystem::path(ReadTestInfo().baseExpectedOutputPath) / filepath));
  }

  // --- //

  transformationData = Transformation();
  transformationData.translation = vrml_proc::parser::model::Vec3f(15.0f, 0.0f, 0.0f);
  transformationData.rotation = vrml_proc::parser::model::Vec4f(1.0f, 0.0f, 0.0f, 0.785398163f);
  matrix = vrml_proc::math::UpdateTransformationMatrix(matrix, transformationData);
  auto result10 = (calculator.Generate3DMesh({std::cref(size)}, matrix)).value();
  {
    GENERATE_TEST_OUTPUT_FILENAME_EXTRA_NAME(filepath, "translationAndXAxisRotation");
    writer.Write(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath, *(result10));
    CHECK(AreBinaryFilesEqual(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath,
        std::filesystem::path(ReadTestInfo().baseExpectedOutputPath) / filepath));
  }

  // --- //

  transformationData = Transformation();
  matrix = TransformationMatrix();
  transformationData.scale = vrml_proc::parser::model::Vec3f(2.0f, 2.0f, 2.0f);
  matrix = vrml_proc::math::UpdateTransformationMatrix(matrix, transformationData);
  matrix = vrml_proc::math::UpdateTransformationMatrix(matrix, transformationData);
  matrix = vrml_proc::math::UpdateTransformationMatrix(matrix, transformationData);
  matrix = vrml_proc::math::UpdateTransformationMatrix(matrix, transformationData);
  auto result11 = (calculator.Generate3DMesh({std::cref(size)}, matrix)).value();
  {
    GENERATE_TEST_OUTPUT_FILENAME_EXTRA_NAME(filepath, "multipleScale");
    writer.Write(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath, *(result11));
    CHECK(AreBinaryFilesEqual(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath,
        std::filesystem::path(ReadTestInfo().baseExpectedOutputPath) / filepath));
  }
}

TEST_CASE("BoxCalculator - invalid", "[invalid]") {
  to_geom::calculator::BoxCalculator calculator = to_geom::calculator::BoxCalculator();

  vrml_proc::parser::model::Vec3f size;
  size.x = 0.0f;
  size.y = 20.0f;
  size.z = 30.0f;

  vrml_proc::math::TransformationMatrix matrix;

  {
    auto result = calculator.Generate3DMesh({std::cref(size)}, matrix);
    REQUIRE(result.has_error());
    CHECK(CheckInnermostError<
        vrml_proc::parser::model::validator::error::NumberOutOfRangeError<vrml_proc::parser::model::float32_t>>(
        result.error()));
    LogError(result.error());
  }

  size.x = 50.5f;
  size.y = 20.0f;
  size.z = -0.0001f;
  {
    auto result = calculator.Generate3DMesh({std::cref(size)}, matrix);
    REQUIRE(result.has_error());
    CHECK(CheckInnermostError<
        vrml_proc::parser::model::validator::error::NumberOutOfRangeError<vrml_proc::parser::model::float32_t>>(
        result.error()));
    LogError(result.error());
  }
}

TEST_CASE("IndexedFaceSetCalculator - valid I.", "[valid]") {
  to_geom::calculator::IndexedFaceSetCalculator calculator = to_geom::calculator::IndexedFaceSetCalculator();

  vrml_proc::parser::model::Int32Array indices;
  vrml_proc::parser::model::Vec3fArray points;

  vrml_proc::math::TransformationMatrix matrix;

  {
    auto result = calculator.Generate3DMesh(std::cref(indices), std::cref(points), matrix);
    REQUIRE(result.has_value());
    REQUIRE(result.value()->is_empty());
  }
}

TEST_CASE("IndexedFaceSetCalculator - invalid I.", "[invalid]") {
  to_geom::calculator::IndexedFaceSetCalculator calculator = to_geom::calculator::IndexedFaceSetCalculator();

  vrml_proc::parser::model::Int32Array indices;
  indices.integers.emplace_back(0);
  indices.integers.emplace_back(1);
  indices.integers.emplace_back(2);
  indices.integers.emplace_back(-1);
  vrml_proc::parser::model::Vec3fArray points;

  vrml_proc::math::TransformationMatrix matrix;

  {
    auto result = calculator.Generate3DMesh(std::cref(indices), std::cref(points), matrix);
    REQUIRE(result.has_error());
    CHECK(CheckInnermostError<vrml_proc::parser::model::validator::error::EmptyArrayError>(result.error()));
    LogError(result.error());
  }
}

TEST_CASE("IndexedFaceSetCalculator - invalid II.", "[invalid]") {
  using vrml_proc::parser::model::Vec3f;

  to_geom::calculator::IndexedFaceSetCalculator calculator = to_geom::calculator::IndexedFaceSetCalculator();

  vrml_proc::parser::model::Int32Array indices;
  indices.integers.emplace_back(-1);

  vrml_proc::parser::model::Vec3fArray points;
  points.vectors.emplace_back(Vec3f(0.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.5f, 0.5f, 1.0f));

  vrml_proc::math::TransformationMatrix matrix;

  {
    auto result = calculator.Generate3DMesh(std::cref(indices), std::cref(points), matrix);
    REQUIRE(result.has_error());
    CHECK(CheckInnermostError<to_geom::calculator::error::InvalidNumberOfCoordinatesForFaceError>(result.error()));
    LogError(result.error());
  }
}

TEST_CASE("IndexedFaceSetCalculator - invalid III.", "[invalid]") {
  using vrml_proc::parser::model::Vec3f;

  to_geom::calculator::IndexedFaceSetCalculator calculator = to_geom::calculator::IndexedFaceSetCalculator();

  vrml_proc::parser::model::Int32Array indices;
  indices.integers.emplace_back(0);
  indices.integers.emplace_back(-1);

  vrml_proc::parser::model::Vec3fArray points;
  points.vectors.emplace_back(Vec3f(0.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.5f, 0.5f, 1.0f));

  vrml_proc::math::TransformationMatrix matrix;

  {
    auto result = calculator.Generate3DMesh(std::cref(indices), std::cref(points), matrix);
    REQUIRE(result.has_error());
    CHECK(CheckInnermostError<to_geom::calculator::error::InvalidNumberOfCoordinatesForFaceError>(result.error()));
    LogError(result.error());
  }
}

TEST_CASE("IndexedFaceSetCalculator - invalid IV.", "[invalid]") {
  using vrml_proc::parser::model::Vec3f;

  to_geom::calculator::IndexedFaceSetCalculator calculator = to_geom::calculator::IndexedFaceSetCalculator();

  vrml_proc::parser::model::Int32Array indices;
  indices.integers.emplace_back(0);
  indices.integers.emplace_back(1);
  indices.integers.emplace_back(-1);

  vrml_proc::parser::model::Vec3fArray points;
  points.vectors.emplace_back(Vec3f(0.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.5f, 0.5f, 1.0f));

  vrml_proc::math::TransformationMatrix matrix;

  {
    auto result = calculator.Generate3DMesh(std::cref(indices), std::cref(points), matrix);
    REQUIRE(result.has_error());
    CHECK(CheckInnermostError<to_geom::calculator::error::InvalidNumberOfCoordinatesForFaceError>(result.error()));
    LogError(result.error());
  }
}

TEST_CASE("IndexedFaceSetCalculator - valid II.", "[invalid]") {
  using vrml_proc::parser::model::Vec3f;

  to_geom::calculator::IndexedFaceSetCalculator calculator = to_geom::calculator::IndexedFaceSetCalculator();

  vrml_proc::parser::model::Int32Array indices;
  indices.integers.emplace_back(0);
  indices.integers.emplace_back(1);
  indices.integers.emplace_back(2);
  indices.integers.emplace_back(-1);
  indices.integers.emplace_back(0);
  indices.integers.emplace_back(2);
  indices.integers.emplace_back(3);
  indices.integers.emplace_back(-1);

  vrml_proc::parser::model::Vec3fArray points;
  points.vectors.emplace_back(Vec3f(0.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.5f, 0.5f, 1.0f));

  vrml_proc::math::TransformationMatrix matrix;

  {
    auto result = calculator.Generate3DMesh(std::cref(indices), std::cref(points), matrix);
    REQUIRE(result.has_value());

    GENERATE_TEST_OUTPUT_FILENAME(filepath);
    to_geom::core::io::StlFileWriter writer;
    writer.Write(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath, *(result.value()));
    CHECK(AreBinaryFilesEqual(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath,
        std::filesystem::path(ReadTestInfo().baseExpectedOutputPath) / filepath));
  }
}

TEST_CASE("IndexedFaceSetCalculator - invalid V.", "[invalid]") {
  using vrml_proc::parser::model::Vec3f;

  to_geom::calculator::IndexedFaceSetCalculator calculator = to_geom::calculator::IndexedFaceSetCalculator();

  vrml_proc::parser::model::Int32Array indices;
  indices.integers.emplace_back(-2);
  indices.integers.emplace_back(1);
  indices.integers.emplace_back(2);
  indices.integers.emplace_back(-1);

  vrml_proc::parser::model::Vec3fArray points;
  points.vectors.emplace_back(Vec3f(0.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.5f, 0.5f, 1.0f));

  vrml_proc::math::TransformationMatrix matrix;

  {
    auto result = calculator.Generate3DMesh(std::cref(indices), std::cref(points), matrix);
    REQUIRE(result.has_error());
    CHECK(CheckInnermostError<vrml_proc::parser::model::validator::error::NumberOutOfRangeError<int32_t>>(
        result.error()));
    LogError(result.error());
  }
}

TEST_CASE("IndexedFaceSetCalculator - invalid VI.", "[invalid]") {
  using vrml_proc::parser::model::Vec3f;

  to_geom::calculator::IndexedFaceSetCalculator calculator = to_geom::calculator::IndexedFaceSetCalculator();

  vrml_proc::parser::model::Int32Array indices;
  indices.integers.emplace_back(5);
  indices.integers.emplace_back(1);
  indices.integers.emplace_back(2);
  indices.integers.emplace_back(-1);

  vrml_proc::parser::model::Vec3fArray points;
  points.vectors.emplace_back(Vec3f(0.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.5f, 0.5f, 1.0f));

  vrml_proc::math::TransformationMatrix matrix;

  {
    auto result = calculator.Generate3DMesh(std::cref(indices), std::cref(points), matrix);
    REQUIRE(result.has_error());
    CHECK(CheckInnermostError<vrml_proc::parser::model::validator::error::NumberOutOfRangeError<int32_t>>(
        result.error()));
    LogError(result.error());
  }
}

TEST_CASE("IndexedFaceSetCalculator - invalid VII.", "[invalid]") {
  using vrml_proc::parser::model::Vec3f;

  to_geom::calculator::IndexedFaceSetCalculator calculator = to_geom::calculator::IndexedFaceSetCalculator();

  vrml_proc::parser::model::Int32Array indices;
  indices.integers.emplace_back(0);
  indices.integers.emplace_back(-2);
  indices.integers.emplace_back(2);
  indices.integers.emplace_back(-1);

  vrml_proc::parser::model::Vec3fArray points;
  points.vectors.emplace_back(Vec3f(0.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.5f, 0.5f, 1.0f));

  vrml_proc::math::TransformationMatrix matrix;

  {
    auto result = calculator.Generate3DMesh(std::cref(indices), std::cref(points), matrix);
    REQUIRE(result.has_error());
    CHECK(CheckInnermostError<vrml_proc::parser::model::validator::error::NumberOutOfRangeError<int32_t>>(
        result.error()));
    LogError(result.error());
  }
}

TEST_CASE("IndexedFaceSetCalculator - invalid VIII.", "[invalid]") {
  using vrml_proc::parser::model::Vec3f;

  to_geom::calculator::IndexedFaceSetCalculator calculator = to_geom::calculator::IndexedFaceSetCalculator();

  vrml_proc::parser::model::Int32Array indices;
  indices.integers.emplace_back(0);
  indices.integers.emplace_back(5);
  indices.integers.emplace_back(2);
  indices.integers.emplace_back(-1);

  vrml_proc::parser::model::Vec3fArray points;
  points.vectors.emplace_back(Vec3f(0.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.5f, 0.5f, 1.0f));

  vrml_proc::math::TransformationMatrix matrix;

  {
    auto result = calculator.Generate3DMesh(std::cref(indices), std::cref(points), matrix);
    REQUIRE(result.has_error());
    CHECK(CheckInnermostError<vrml_proc::parser::model::validator::error::NumberOutOfRangeError<int32_t>>(
        result.error()));
    LogError(result.error());
  }
}

TEST_CASE("IndexedFaceSetCalculator - invalid IX.", "[invalid]") {
  using vrml_proc::parser::model::Vec3f;

  to_geom::calculator::IndexedFaceSetCalculator calculator = to_geom::calculator::IndexedFaceSetCalculator();

  vrml_proc::parser::model::Int32Array indices;
  indices.integers.emplace_back(0);
  indices.integers.emplace_back(1);
  indices.integers.emplace_back(-2);
  indices.integers.emplace_back(-1);

  vrml_proc::parser::model::Vec3fArray points;
  points.vectors.emplace_back(Vec3f(0.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.5f, 0.5f, 1.0f));

  vrml_proc::math::TransformationMatrix matrix;

  {
    auto result = calculator.Generate3DMesh(std::cref(indices), std::cref(points), matrix);
    REQUIRE(result.has_error());
    CHECK(CheckInnermostError<vrml_proc::parser::model::validator::error::NumberOutOfRangeError<int32_t>>(
        result.error()));
    LogError(result.error());
  }
}

TEST_CASE("IndexedFaceSetCalculator - invalid X.", "[invalid]") {
  using vrml_proc::parser::model::Vec3f;

  to_geom::calculator::IndexedFaceSetCalculator calculator = to_geom::calculator::IndexedFaceSetCalculator();

  vrml_proc::parser::model::Int32Array indices;
  indices.integers.emplace_back(0);
  indices.integers.emplace_back(1);
  indices.integers.emplace_back(5);
  indices.integers.emplace_back(-1);

  vrml_proc::parser::model::Vec3fArray points;
  points.vectors.emplace_back(Vec3f(0.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.5f, 0.5f, 1.0f));

  vrml_proc::math::TransformationMatrix matrix;

  {
    auto result = calculator.Generate3DMesh(std::cref(indices), std::cref(points), matrix);
    REQUIRE(result.has_error());
    CHECK(CheckInnermostError<vrml_proc::parser::model::validator::error::NumberOutOfRangeError<int32_t>>(
        result.error()));
    LogError(result.error());
  }
}

TEST_CASE("IndexedFaceSetCalculator - valid III.", "[valid]") {
  using vrml_proc::parser::model::Vec3f;

  to_geom::calculator::IndexedFaceSetCalculator calculator = to_geom::calculator::IndexedFaceSetCalculator();

  vrml_proc::parser::model::Int32Array indices;
  indices.integers.emplace_back(0);
  indices.integers.emplace_back(1);
  indices.integers.emplace_back(2);
  indices.integers.emplace_back(-1);
  indices.integers.emplace_back(0);
  indices.integers.emplace_back(2);
  indices.integers.emplace_back(3);
  indices.integers.emplace_back(-1);
  indices.integers.emplace_back(0);
  indices.integers.emplace_back(1);
  indices.integers.emplace_back(4);
  indices.integers.emplace_back(-1);
  indices.integers.emplace_back(1);
  indices.integers.emplace_back(2);
  indices.integers.emplace_back(4);
  indices.integers.emplace_back(-1);
  indices.integers.emplace_back(2);
  indices.integers.emplace_back(3);
  indices.integers.emplace_back(4);
  indices.integers.emplace_back(-1);
  indices.integers.emplace_back(3);
  indices.integers.emplace_back(0);
  indices.integers.emplace_back(4);
  indices.integers.emplace_back(-1);

  vrml_proc::parser::model::Vec3fArray points;
  points.vectors.emplace_back(Vec3f(0.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.5f, 0.5f, 1.0f));

  vrml_proc::math::TransformationMatrix matrix;

  {
    auto result = calculator.Generate3DMesh(std::cref(indices), std::cref(points), matrix);
    REQUIRE(result.has_value());
    GENERATE_TEST_OUTPUT_FILENAME(filepath);
    to_geom::core::io::StlFileWriter writer;
    writer.Write(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath, *(result.value()));
    CHECK(AreBinaryFilesEqual(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath,
        std::filesystem::path(ReadTestInfo().baseExpectedOutputPath) / filepath));
  }
}

TEST_CASE("AlphaShapeCalculator - valid I.", "[valid]") {
  using vrml_proc::parser::model::Vec3f;

  std::vector<Vec3f> testPoints = {{5.0f, 0.0f, 1.0f}, {1.0f, 9.0f, 1.0f}, {1.0f, 1.0f, -5.0f}, {0.0f, 10.0f, 10.0f},
      {-11.0f, 0.0f, 12.0f}, {1.0f, 0.0f, 9.0f}, {1.0f, 7.0f, 8.0f}, {0.0f, -1.0f, 0.0f}, {0.25f, 0.25f, 0.25f},
      {0.75f, 0.25f, 0.25f}, {0.75f, 0.75f, 0.25f}, {0.25f, 0.75f, 0.25f}, {0.25f, 0.25f, 0.75f}, {0.75f, 0.25f, 0.75f},
      {0.75f, 0.75f, 0.75f}, {0.25f, 0.75f, 0.75f}, {-0.5f, 0.5f, 0.5f}, {5.0f, -0.5f, 0.0f}, {9.0f, 0.5f, -1.0f},
      {5.5f, -4.0f, 10.5f}, {12.5f, 1.0f, 8.5f}, {-9.0f, 0.5f, 69.5f}, {1.0f, 7.5f, 2.5f}};

  vrml_proc::parser::model::Vec3fArray pointCloud;
  pointCloud.vectors.insert(
      pointCloud.vectors.end(), std::make_move_iterator(testPoints.begin()), std::make_move_iterator(testPoints.end()));
  vrml_proc::math::TransformationMatrix matrix;

  {
    auto result = to_geom::calculator::AlphaShapeCalculator::Generate3DAlphaShapeMeshForPointCloud(
        std::cref(pointCloud), 0.5, matrix);

    REQUIRE(result.has_value());
    GENERATE_TEST_OUTPUT_FILENAME(filepath);
    to_geom::core::io::StlFileWriter writer;
    writer.Write(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath, *(result.value()));
    CHECK(HaveSimiliarSizes(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath,
        std::filesystem::path(ReadTestInfo().baseExpectedOutputPath) / filepath, 100));
  }
}

TEST_CASE("IndexedFaceSetCalculator (only triangular faces without their range check) - valid I.", "[valid]") {
  to_geom::calculator::IndexedFaceSetCalculator calculator = to_geom::calculator::IndexedFaceSetCalculator();

  vrml_proc::parser::model::Int32Array indices;
  vrml_proc::parser::model::Vec3fArray points;

  vrml_proc::math::TransformationMatrix matrix;

  {
    auto result = calculator.Generate3DMesh(std::cref(indices), std::cref(points), matrix, false, true);
    REQUIRE(result.has_value());
    REQUIRE(result.value()->is_empty());
  }
}

TEST_CASE("IndexedFaceSetCalculator (only triangular faces without their range check) - valid II.", "[valid]") {
  using vrml_proc::parser::model::Vec3f;

  to_geom::calculator::IndexedFaceSetCalculator calculator = to_geom::calculator::IndexedFaceSetCalculator();

  vrml_proc::parser::model::Int32Array indices;
  indices.integers.emplace_back(0);
  indices.integers.emplace_back(1);
  indices.integers.emplace_back(2);
  indices.integers.emplace_back(-1);
  indices.integers.emplace_back(0);
  indices.integers.emplace_back(2);
  indices.integers.emplace_back(3);
  indices.integers.emplace_back(-1);

  vrml_proc::parser::model::Vec3fArray points;
  points.vectors.emplace_back(Vec3f(0.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.5f, 0.5f, 1.0f));

  vrml_proc::math::TransformationMatrix matrix;

  {
    auto result = calculator.Generate3DMesh(std::cref(indices), std::cref(points), matrix, false, true);
    REQUIRE(result.has_value());

    GENERATE_TEST_OUTPUT_FILENAME(filepath);
    to_geom::core::io::StlFileWriter writer;
    writer.Write(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath, *(result.value()));
    CHECK(AreBinaryFilesEqual(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath,
        std::filesystem::path(ReadTestInfo().baseExpectedOutputPath) / filepath));
  }
}

TEST_CASE("IndexedFaceSetCalculator (only triangular faces without their range check) - valid III.", "[valid]") {
  using vrml_proc::parser::model::Vec3f;

  to_geom::calculator::IndexedFaceSetCalculator calculator = to_geom::calculator::IndexedFaceSetCalculator();

  vrml_proc::parser::model::Int32Array indices;
  indices.integers.emplace_back(0);
  indices.integers.emplace_back(1);
  indices.integers.emplace_back(2);
  indices.integers.emplace_back(-1);
  indices.integers.emplace_back(0);
  indices.integers.emplace_back(2);
  indices.integers.emplace_back(3);
  indices.integers.emplace_back(-1);
  indices.integers.emplace_back(1);
  indices.integers.emplace_back(0);
  indices.integers.emplace_back(4);
  indices.integers.emplace_back(-1);
  indices.integers.emplace_back(2);
  indices.integers.emplace_back(1);
  indices.integers.emplace_back(4);
  indices.integers.emplace_back(-1);
  indices.integers.emplace_back(3);
  indices.integers.emplace_back(2);
  indices.integers.emplace_back(4);
  indices.integers.emplace_back(-1);
  indices.integers.emplace_back(0);
  indices.integers.emplace_back(3);
  indices.integers.emplace_back(4);
  indices.integers.emplace_back(-1);

  vrml_proc::parser::model::Vec3fArray points;
  points.vectors.emplace_back(Vec3f(0.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.5f, 0.5f, 1.0f));

  vrml_proc::math::TransformationMatrix matrix;

  {
    auto result = calculator.Generate3DMesh(std::cref(indices), std::cref(points), matrix, false, true);
    REQUIRE(result.has_value());
    GENERATE_TEST_OUTPUT_FILENAME(filepath);
    to_geom::core::io::StlFileWriter writer;
    writer.Write(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath, *(result.value()));
    CHECK(AreBinaryFilesEqual(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath,
        std::filesystem::path(ReadTestInfo().baseExpectedOutputPath) / filepath));
  }
}

TEST_CASE("IndexedFaceSetCalculator (only triangular faces with their range check) - valid I.", "[valid]") {
  to_geom::calculator::IndexedFaceSetCalculator calculator = to_geom::calculator::IndexedFaceSetCalculator();

  vrml_proc::parser::model::Int32Array indices;
  vrml_proc::parser::model::Vec3fArray points;

  vrml_proc::math::TransformationMatrix matrix;

  {
    auto result = calculator.Generate3DMesh(std::cref(indices), std::cref(points), matrix, true, true);
    REQUIRE(result.has_value());
    REQUIRE(result.value()->is_empty());
  }
}

TEST_CASE("IndexedFaceSetCalculator (only triangular faces with their range check) - valid II.", "[valid]") {
  using vrml_proc::parser::model::Vec3f;

  to_geom::calculator::IndexedFaceSetCalculator calculator = to_geom::calculator::IndexedFaceSetCalculator();

  vrml_proc::parser::model::Int32Array indices;
  indices.integers.emplace_back(0);
  indices.integers.emplace_back(1);
  indices.integers.emplace_back(2);
  indices.integers.emplace_back(-1);
  indices.integers.emplace_back(0);
  indices.integers.emplace_back(2);
  indices.integers.emplace_back(3);
  indices.integers.emplace_back(-1);

  vrml_proc::parser::model::Vec3fArray points;
  points.vectors.emplace_back(Vec3f(0.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.5f, 0.5f, 1.0f));

  vrml_proc::math::TransformationMatrix matrix;

  {
    auto result = calculator.Generate3DMesh(std::cref(indices), std::cref(points), matrix, true, true);
    REQUIRE(result.has_value());

    GENERATE_TEST_OUTPUT_FILENAME(filepath);
    to_geom::core::io::StlFileWriter writer;
    writer.Write(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath, *(result.value()));
    CHECK(AreBinaryFilesEqual(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath,
        std::filesystem::path(ReadTestInfo().baseExpectedOutputPath) / filepath));
  }
}

TEST_CASE("IndexedFaceSetCalculator (only triangular faces with their range check) - valid III.", "[valid]") {
  using vrml_proc::parser::model::Vec3f;

  to_geom::calculator::IndexedFaceSetCalculator calculator = to_geom::calculator::IndexedFaceSetCalculator();

  vrml_proc::parser::model::Int32Array indices;
  indices.integers.emplace_back(0);
  indices.integers.emplace_back(1);
  indices.integers.emplace_back(2);
  indices.integers.emplace_back(-1);
  indices.integers.emplace_back(0);
  indices.integers.emplace_back(2);
  indices.integers.emplace_back(3);
  indices.integers.emplace_back(-1);
  indices.integers.emplace_back(1);
  indices.integers.emplace_back(0);
  indices.integers.emplace_back(4);
  indices.integers.emplace_back(-1);
  indices.integers.emplace_back(2);
  indices.integers.emplace_back(1);
  indices.integers.emplace_back(4);
  indices.integers.emplace_back(-1);
  indices.integers.emplace_back(3);
  indices.integers.emplace_back(2);
  indices.integers.emplace_back(4);
  indices.integers.emplace_back(-1);
  indices.integers.emplace_back(0);
  indices.integers.emplace_back(3);
  indices.integers.emplace_back(4);
  indices.integers.emplace_back(-1);

  vrml_proc::parser::model::Vec3fArray points;
  points.vectors.emplace_back(Vec3f(0.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.5f, 0.5f, 1.0f));

  vrml_proc::math::TransformationMatrix matrix;

  {
    auto result = calculator.Generate3DMesh(std::cref(indices), std::cref(points), matrix, true, true);
    REQUIRE(result.has_value());
    GENERATE_TEST_OUTPUT_FILENAME(filepath);
    to_geom::core::io::StlFileWriter writer;
    writer.Write(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath, *(result.value()));
    CHECK(AreBinaryFilesEqual(std::filesystem::path(ReadTestInfo().baseOutputPath) / filepath,
        std::filesystem::path(ReadTestInfo().baseExpectedOutputPath) / filepath));
  }
}

// ---------------------------------

TEST_CASE("IndexedFaceSetCalculator (only triangular faces with their range check) - invalid I.", "[invalid]") {
  to_geom::calculator::IndexedFaceSetCalculator calculator = to_geom::calculator::IndexedFaceSetCalculator();

  vrml_proc::parser::model::Int32Array indices;
  indices.integers.emplace_back(0);
  indices.integers.emplace_back(1);
  indices.integers.emplace_back(2);
  indices.integers.emplace_back(-1);
  vrml_proc::parser::model::Vec3fArray points;

  vrml_proc::math::TransformationMatrix matrix;

  {
    auto result = calculator.Generate3DMesh(std::cref(indices), std::cref(points), matrix, true, true);
    REQUIRE(result.has_error());
    CHECK(CheckInnermostError<vrml_proc::parser::model::validator::error::EmptyArrayError>(result.error()));
    LogError(result.error());
  }
}

TEST_CASE("IndexedFaceSetCalculator (only triangular faces with their range check) - invalid II.", "[invalid]") {
  using vrml_proc::parser::model::Vec3f;

  to_geom::calculator::IndexedFaceSetCalculator calculator = to_geom::calculator::IndexedFaceSetCalculator();

  vrml_proc::parser::model::Int32Array indices;
  indices.integers.emplace_back(-2);
  indices.integers.emplace_back(1);
  indices.integers.emplace_back(2);
  indices.integers.emplace_back(-1);

  vrml_proc::parser::model::Vec3fArray points;
  points.vectors.emplace_back(Vec3f(0.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.5f, 0.5f, 1.0f));

  vrml_proc::math::TransformationMatrix matrix;

  {
    auto result = calculator.Generate3DMesh(std::cref(indices), std::cref(points), matrix, true, true);
    REQUIRE(result.has_error());
    CHECK(CheckInnermostError<vrml_proc::parser::model::validator::error::NumberOutOfRangeError<int32_t>>(
        result.error()));
    LogError(result.error());
  }
}

TEST_CASE("IndexedFaceSetCalculator (only triangular faces with their range check) - invalid III.", "[invalid]") {
  using vrml_proc::parser::model::Vec3f;

  to_geom::calculator::IndexedFaceSetCalculator calculator = to_geom::calculator::IndexedFaceSetCalculator();

  vrml_proc::parser::model::Int32Array indices;
  indices.integers.emplace_back(5);
  indices.integers.emplace_back(1);
  indices.integers.emplace_back(2);
  indices.integers.emplace_back(-1);

  vrml_proc::parser::model::Vec3fArray points;
  points.vectors.emplace_back(Vec3f(0.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.5f, 0.5f, 1.0f));

  vrml_proc::math::TransformationMatrix matrix;

  {
    auto result = calculator.Generate3DMesh(std::cref(indices), std::cref(points), matrix, true, true);
    REQUIRE(result.has_error());
    CHECK(CheckInnermostError<vrml_proc::parser::model::validator::error::NumberOutOfRangeError<int32_t>>(
        result.error()));
    LogError(result.error());
  }
}

TEST_CASE("IndexedFaceSetCalculator (only triangular faces with their range check) - invalid IV.", "[invalid]") {
  using vrml_proc::parser::model::Vec3f;

  to_geom::calculator::IndexedFaceSetCalculator calculator = to_geom::calculator::IndexedFaceSetCalculator();

  vrml_proc::parser::model::Int32Array indices;
  indices.integers.emplace_back(0);
  indices.integers.emplace_back(-2);
  indices.integers.emplace_back(2);
  indices.integers.emplace_back(-1);

  vrml_proc::parser::model::Vec3fArray points;
  points.vectors.emplace_back(Vec3f(0.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.5f, 0.5f, 1.0f));

  vrml_proc::math::TransformationMatrix matrix;

  {
    auto result = calculator.Generate3DMesh(std::cref(indices), std::cref(points), matrix, true, true);
    REQUIRE(result.has_error());
    CHECK(CheckInnermostError<vrml_proc::parser::model::validator::error::NumberOutOfRangeError<int32_t>>(
        result.error()));
    LogError(result.error());
  }
}

TEST_CASE("IndexedFaceSetCalculator (only triangular faces with their range check) - invalid V.", "[invalid]") {
  using vrml_proc::parser::model::Vec3f;

  to_geom::calculator::IndexedFaceSetCalculator calculator = to_geom::calculator::IndexedFaceSetCalculator();

  vrml_proc::parser::model::Int32Array indices;
  indices.integers.emplace_back(0);
  indices.integers.emplace_back(5);
  indices.integers.emplace_back(2);
  indices.integers.emplace_back(-1);

  vrml_proc::parser::model::Vec3fArray points;
  points.vectors.emplace_back(Vec3f(0.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.5f, 0.5f, 1.0f));

  vrml_proc::math::TransformationMatrix matrix;

  {
    auto result = calculator.Generate3DMesh(std::cref(indices), std::cref(points), matrix, true, true);
    REQUIRE(result.has_error());
    CHECK(CheckInnermostError<vrml_proc::parser::model::validator::error::NumberOutOfRangeError<int32_t>>(
        result.error()));
    LogError(result.error());
  }
}

TEST_CASE("IndexedFaceSetCalculator (only triangular faces with their range check) - invalid VI.", "[invalid]") {
  using vrml_proc::parser::model::Vec3f;

  to_geom::calculator::IndexedFaceSetCalculator calculator = to_geom::calculator::IndexedFaceSetCalculator();

  vrml_proc::parser::model::Int32Array indices;
  indices.integers.emplace_back(0);
  indices.integers.emplace_back(1);
  indices.integers.emplace_back(-2);
  indices.integers.emplace_back(-1);

  vrml_proc::parser::model::Vec3fArray points;
  points.vectors.emplace_back(Vec3f(0.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.5f, 0.5f, 1.0f));

  vrml_proc::math::TransformationMatrix matrix;

  {
    auto result = calculator.Generate3DMesh(std::cref(indices), std::cref(points), matrix, true, true);
    REQUIRE(result.has_error());
    CHECK(CheckInnermostError<vrml_proc::parser::model::validator::error::NumberOutOfRangeError<int32_t>>(
        result.error()));
    LogError(result.error());
  }
}

TEST_CASE("IndexedFaceSetCalculator (only triangular faces with their range check) - invalid VII.", "[invalid]") {
  using vrml_proc::parser::model::Vec3f;

  to_geom::calculator::IndexedFaceSetCalculator calculator = to_geom::calculator::IndexedFaceSetCalculator();

  vrml_proc::parser::model::Int32Array indices;
  indices.integers.emplace_back(0);
  indices.integers.emplace_back(1);
  indices.integers.emplace_back(5);
  indices.integers.emplace_back(-1);

  vrml_proc::parser::model::Vec3fArray points;
  points.vectors.emplace_back(Vec3f(0.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.5f, 0.5f, 1.0f));

  vrml_proc::math::TransformationMatrix matrix;

  {
    auto result = calculator.Generate3DMesh(std::cref(indices), std::cref(points), matrix, true, true);
    REQUIRE(result.has_error());
    CHECK(CheckInnermostError<vrml_proc::parser::model::validator::error::NumberOutOfRangeError<int32_t>>(
        result.error()));
    LogError(result.error());
  }
}
