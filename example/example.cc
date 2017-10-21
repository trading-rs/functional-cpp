#include <fp/fp.hpp>
using namespace fp;

#include <iostream>
using namespace std;

int main(int argc, char** argv) {
  function<int(string)> length = [](string s){ return s.size(); };
  function<int(int)> square = [](int i) { return i*i; };

  cout << compose(length, square)("cleantha") << endl;

  Maybe<string> just_str("cleantha");
  Maybe<string> nothing;

  cout << "Untouched Maybes:" << endl;
  cout << "\t" << just_str << endl;
  cout << "\t" << nothing << endl;

  cout << "Maybes fmapped with length:" << endl;
  cout << "\t" << (just_str ^ length) << endl;
  cout << "\t" << (nothing ^ length) << endl;
}
