#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <optional>
#include <string>

#include "CommentSkipper.hpp"
#include "Vec3f.hpp"
#include "Vec3fGrammar.hpp"
#include "test_data/Vec3fGrammarTestDataset.hpp"

static std::optional<vrml_proc::parser::Vec3f> ParseVec3f(std::string& text) {
  auto iterator = text.begin();

  vrml_proc::parser::grammar::Vec3fGrammar<const char*, vrml_proc::parser::grammar::CommentSkipper> grammar;
  vrml_proc::parser::Vec3f data;
  vrml_proc::parser::grammar::CommentSkipper skipper;

  bool success = boost::spirit::qi::phrase_parse(text.c_str(), text.c_str() + text.size(), grammar, skipper, data);
  if (success) {
    return data;
  }

  return {};
}

TEST_CASE("Parse Vec3f - Valid Input", "[parsing][valid]") {
  auto parseResult = ParseVec3f(vec3f);
  REQUIRE(parseResult.has_value());

  CHECK_THAT(parseResult.value().x, Catch::Matchers::WithinAbs(100.001001, 0.001));
  CHECK_THAT(parseResult.value().y, Catch::Matchers::WithinAbs(5.55317, 0.001));
  CHECK_THAT(parseResult.value().z, Catch::Matchers::WithinAbs(-0.0305561, 0.001));
}

TEST_CASE("Parse Vec3f - Valid Input", "[parsing][invalid]") {
  auto parseResult = ParseVec3f(invalidVec3f);
  REQUIRE_FALSE(parseResult.has_value());
}