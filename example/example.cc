#include <fp/fp.hpp>
using namespace fp;

#include <iostream>
using namespace std;

#include <json.hpp>
using json = nlohmann::json;

#include <map>
using namespace std;

int main(int argc, char** argv) {
  function<int(string)> length = [](string s){ return s.size(); };
  function<int(int)> square = [](int i) { return i*i; };
  function<long(json)> get_server_time = [](const auto &j) {
    map<string, long> m = j;
    return j["serverTime"];
  };
  function<long(json)> get_server_time2 = [](const json &j) {
    return j["serverTime"].get<long>();
  };
  function<Maybe<long>(json)> get_server_time_maybe = [](const auto &j) {
    return Maybe<long>(j["serverTime"].template get<long>());
  };

  cout << compose(length, square)("cleantha") << endl;

  Maybe<string> just_str("cleantha");
  Maybe<string> nothing;
  json j = json::parse("{\"serverTime\": 1499827319559}");
  Maybe<json> just_json(j);

  cout << "Untouched Maybes:" << endl;
  cout << "\t" << just_str << endl;
  cout << "\t" << nothing << endl;

  cout << "Maybes fmapped with length:" << endl;
  cout << "\t" << (just_str ^ length) << endl;
  cout << "\t" << (nothing ^ length) << endl;

  cout << (just_json ^ get_server_time) << endl;
  cout << (just_json ^ get_server_time2) << endl;

  cout << (just_json >>= get_server_time_maybe) << endl;
}
