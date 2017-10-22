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

TEST_CASE("Test function composition") {
  REQUIRE(compose(length, square)("cleantha") == 64);
}

TEST_CASE("Test maybe functor") {
  json j = json::parse("{\"serverTime\": 1499827319559}");
  Maybe<json> just_json(j);

  Maybe<string> just_str("cleantha");
  Maybe<string> nothing;

  REQUIRE((just_str ^ length) == (Maybe<int>(8)));
  REQUIRE((nothing ^ length).isNothing());
  REQUIRE((nothing ^ length) == Maybe<int>());
  REQUIRE((nothing ^ length) == Nothing<int>);
  REQUIRE((just_json ^ get_server_time) == (Maybe<long>(1499827319559)));
  REQUIRE((just_json ^ get_server_time2) == Maybe<long>(1499827319559));
}
