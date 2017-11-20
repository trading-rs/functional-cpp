#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include <fp/fp.hpp>
using namespace fp;

#include <json.hpp>
using json = nlohmann::json;

#include <map>
using namespace std;

function<int(string)> length = [](const auto &s){ return s.size(); };
function<int(int)> square = [](const auto &i) { return i * i; };
function<long(json)> get_server_time = [](const auto &j) {
  map<string, long> m = j;
  return j["serverTime"];
};
function<long(json)> get_server_time2 = [](const json &j) {
  return j["serverTime"].get<long>();
};
function<Maybe<json>(json)> print_json = [](const auto &j) {
  cout << j.dump(2) << endl;
  return Nothing<json>;
};
function<Either<string, int>(int)> either_f = [] (int x) {
  if (x == 2) {
    return Either<string, int>::left("Cannot deal with  2.");
  }
  if (x == 12) {
    return Either<string, int>::left("Cannot deal with 12.");
  }
  return Either<string, int>::right(x * 2);
};

TEST_CASE("Test function composition") {
  REQUIRE(compose(length, square)("cleantha") == 64);
}

TEST_CASE("Test maybe functor") {
  json j = json::parse("{\"serverTime\": 1499827319559}");
  Maybe<json> just_json = return_<Maybe>(j);
  Maybe<json> just_json2(j);
  REQUIRE(just_json == just_json2);

  Maybe<string> just_str("cleantha");
  Maybe<string> nothing;

  REQUIRE((just_str ^ length) == (Maybe<int>(8)));
  REQUIRE((nothing ^ length).isNothing());
  REQUIRE((nothing ^ length) == Maybe<int>());
  REQUIRE((nothing ^ length) == Nothing<int>);
  REQUIRE((just_json >>= print_json) == Nothing<json>);
  REQUIRE((Nothing<json> >>= print_json) == Nothing<json>);
  REQUIRE((just_json ^ get_server_time) == (Maybe<long>(1499827319559)));
  REQUIRE((just_json ^ get_server_time2) == Maybe<long>(1499827319559));
}

TEST_CASE("Test fromMaybe member function") {
  Maybe<string> just_str("cleantha");
  Maybe<string> nothing;

  REQUIRE(just_str.fromMaybe("33") == "cleantha");
  REQUIRE(nothing.fromMaybe("33") == "33");
}

TEST_CASE("Test maybe static function") {
  Maybe<string> just_str("cleantha");
  Maybe<string> nothing;

  REQUIRE(maybe(3, length, just_str) == 8);
  REQUIRE(maybe(3, length, nothing) == 3);
}


TEST_CASE("TEST either monad") {
  auto e1 =Either<string, int>::right(3);
  auto e2 = Either<string, int>::right(4);
  auto e3 = Either<string, int>::right(32);
  auto e4 = Either<string, int>::right(3);
  auto e5 = Either<string, int>::left("Cannot deal with 12.");
  REQUIRE(e1 == e4);
  REQUIRE(e1 != e2);

  auto r1 = ((e1 |= either_f) |= either_f) |= either_f;
  auto r2 = ((e2 |= either_f) |= either_f) |= either_f;
  REQUIRE(r1 == e5);
  REQUIRE(r2 == e3);
}
