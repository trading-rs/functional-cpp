#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include <fp/fp.hpp>
using namespace fp;

function<int(string)> length = [](string s){ return s.size(); };
function<int(int)> square = [](int i) { return i*i; };

TEST_CASE("Test function composition") {
  REQUIRE(compose(length, square)("cleantha") == 64);
}

TEST_CASE("Test maybe functor") {
  Maybe<string> just_str("cleantha");
  Maybe<string> nothing;

  REQUIRE((just_str ^ length) == (Maybe<int>(8)));
  REQUIRE((nothing ^ length).isNothing());
}
