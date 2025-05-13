#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <optional>
#include <string>

#include "CommentSkipper.hpp"
#include "test_data/Vec3fArrayGrammarTestDataset.hpp"
#include "Vec3fArray.hpp"
#include "Vec3fArrayGrammar.hpp"

static std::optional<vrml_proc::parser::model::Vec3fArray> ParseVec3fArray(std::string& text) {
  auto iterator = text.begin();

  vrml_proc::parser::grammar::Vec3fArrayGrammar<const char*, vrml_proc::parser::grammar::CommentSkipper> grammar;
  vrml_proc::parser::model::Vec3fArray data;
  vrml_proc::parser::grammar::CommentSkipper skipper;

  bool success = boost::spirit::qi::phrase_parse(text.c_str(), text.c_str() + text.size(), grammar, skipper, data);
  if (success) {
    return data;
  }

  return {};
}

TEST_CASE("Parse Vec3fArray - Valid Input", "[parsing][valid]") {
  auto parseResult = ParseVec3fArray(vec3fArray);
  REQUIRE(parseResult.has_value());

  CHECK_THAT(parseResult.value().vectors.at(0).x, Catch::Matchers::WithinAbs(8.59816, 0.001));
  CHECK_THAT(parseResult.value().vectors.at(0).y, Catch::Matchers::WithinAbs(5.55317, 0.001));
  CHECK_THAT(parseResult.value().vectors.at(0).z, Catch::Matchers::WithinAbs(-3.05561, 0.001));

  CHECK_THAT(parseResult.value().vectors.at(1).x, Catch::Matchers::WithinAbs(8.59816f, 0.001f));
  CHECK_THAT(parseResult.value().vectors.at(1).y, Catch::Matchers::WithinAbs(2.49756f, 0.001f));
  CHECK_THAT(parseResult.value().vectors.at(1).z, Catch::Matchers::WithinAbs(0.0f, 0.0f));

  CHECK_THAT(parseResult.value().vectors.at(2).x, Catch::Matchers::WithinAbs(8.59816f, 0.001f));
  CHECK_THAT(parseResult.value().vectors.at(2).y, Catch::Matchers::WithinAbs(2.49756f, 0.001f));
  CHECK_THAT(parseResult.value().vectors.at(2).z, Catch::Matchers::WithinAbs(-3.05561f, 0.001f));

  CHECK_THAT(parseResult.value().vectors.at(3).x, Catch::Matchers::WithinAbs(8.59816f, 0.001f));
  CHECK_THAT(parseResult.value().vectors.at(3).y, Catch::Matchers::WithinAbs(0.555317f, 0.001f));
  CHECK_THAT(parseResult.value().vectors.at(3).z, Catch::Matchers::WithinAbs(-3.05561f, 0.001f));
}

TEST_CASE("Parse Vec3fArray - Invalid Input", "[parsing][invalid]") {
  auto parseResult = ParseVec3fArray(invalidVec3fArray);
  REQUIRE_FALSE(parseResult.has_value());
}