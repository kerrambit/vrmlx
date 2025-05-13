#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <optional>
#include <string>

#include "CommentSkipper.hpp"
#include "test_data/Vec4fGrammarTestDataset.hpp"
#include "Vec4f.hpp"
#include "Vec4fGrammar.hpp"

static std::optional<vrml_proc::parser::Vec4f> ParseVec4f(std::string& text) {
  auto iterator = text.begin();

  vrml_proc::parser::grammar::Vec4fGrammar<const char*, vrml_proc::parser::grammar::CommentSkipper> grammar;
  vrml_proc::parser::Vec4f data;
  vrml_proc::parser::grammar::CommentSkipper skipper;

  bool success = boost::spirit::qi::phrase_parse(text.c_str(), text.c_str() + text.size(), grammar, skipper, data);
  if (success) {
    return data;
  }

  return {};
}

TEST_CASE("Parse Vec4f - Valid Input", "[parsing][valid]") {
  auto parseResult = ParseVec4f(vecf4);
  REQUIRE(parseResult.has_value());

  CHECK_THAT(parseResult.value().x, Catch::Matchers::WithinAbs(100.001001, 0.001));
  CHECK_THAT(parseResult.value().y, Catch::Matchers::WithinAbs(5.55317, 0.001));
  CHECK_THAT(parseResult.value().z, Catch::Matchers::WithinAbs(-0.0305561, 0.001));
  CHECK_THAT(parseResult.value().w, Catch::Matchers::WithinAbs(1.00, 0.0));
}

TEST_CASE("Parse Vec4f - Ineteger input", "[parsing][valid]") {
  auto parseResult = ParseVec4f(vecf4AsInts);
  REQUIRE(parseResult.has_value());

  CHECK_THAT(parseResult.value().x, Catch::Matchers::WithinAbs(0.0, 0.0));
  CHECK_THAT(parseResult.value().y, Catch::Matchers::WithinAbs(0.0, 0.0));
  CHECK_THAT(parseResult.value().z, Catch::Matchers::WithinAbs(1.0, 0.0));
  CHECK_THAT(parseResult.value().w, Catch::Matchers::WithinAbs(0.00, 0.0));
}