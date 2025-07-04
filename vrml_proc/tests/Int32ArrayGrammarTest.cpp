#include <catch2/catch_test_macros.hpp>

#include <optional>
#include <string>

#include "CommentSkipper.hpp"
#include "test_data/Int32ArrayGrammarTestDataset.hpp"
#include "Int32Array.hpp"
#include "Int32ArrayGrammar.hpp"

static std::optional<vrml_proc::parser::model::Int32Array> ParseInt32Array(std::string& text) {
  auto iterator = text.begin();

  vrml_proc::parser::grammar::Int32ArrayGrammar<const char*, vrml_proc::parser::grammar::CommentSkipper> grammar;
  vrml_proc::parser::model::Int32Array data;
  vrml_proc::parser::grammar::CommentSkipper skipper;

  bool success = boost::spirit::qi::phrase_parse(text.c_str(), text.c_str() + text.size(), grammar, skipper, data);
  if (success) {
    return data;
  }

  return {};
}

TEST_CASE("Parse Vec3fArray - Valid Input", "[parsing][valid]") {
  auto parseResult = ParseInt32Array(int32Array);
  REQUIRE(parseResult.has_value());

  std::vector<int32_t> expectedValues = {2, 0, 1, -1, 5, 3, 4, -1, 8, 6, 7, -1, 11, 9, 10, -1, 7, 9, 11, -1, 8, 7, 11,
      -1, 8, 11, 4, -1, 8, 4, 3, -1, 14, 12, 13, -1, 5, 16, 15, -1, 3, 5, 15, -1, 13, 15, 16, -1, 13, 16, 14, -1, 14,
      16, 17, -1, 8, 19, 18, -1, 6, 8, 18, -1, 1, 18, 19, -1, 2, 1, 19, -1, 2, 19, 13, -1, 2, 13, 12, -1, 15, 8, 3, -1,
      19, 8, 15, -1, 13, 19, 15, -1, 22, 20, 21, -1, 25, 23, 24, -1, 28, 26, 27, -1, 31, 29, 30, -1, 27, 29, 31, -1, 28,
      27, 31, -1, 28, 31, 24, -1, 28, 24, 23, -1, 34, 32, 33, -1, 24, 32, 34, -1, 24, 34, 25, -1, 25, 34, 35, -1, 37,
      34, 33, -1, 36, 37, 33, -1, 20, 37, 21, -1, 20, 35, 37, -1, 35, 34, 37, -1, 32, 31, 30, -1, 32, 30, 33, -1, 33,
      30, 38, -1, 24, 31, 32, -1, 33, 38, 36, -1, 38, 39, 36, -1, 42, 40, 41, -1, 42, 44, 43, -1, 40, 42, 43, -1, 47,
      45, 46, -1, 47, 49, 48, -1, 45, 47, 48, -1, 46, 43, 44, -1, 46, 44, 47, -1};

  REQUIRE(parseResult.value().integers.size() == expectedValues.size());

  for (size_t i = 0; i < parseResult.value().integers.size(); ++i) {
    CHECK(parseResult.value().integers.at(i) == expectedValues.at(i));
  }
}