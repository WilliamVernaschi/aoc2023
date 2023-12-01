#include <iostream>
#include <algorithm>
#include <cassert>
#include <map>
#include <optional>
#include <functional>

using namespace std;

int main(){


  map<string, int> lookup = {
    
    // comment this section
    {"one", 1},
    {"two", 2},
    {"three", 3},
    {"four", 4},
    {"five", 5},
    {"six", 6},
    {"seven", 7},
    {"eight", 8},
    {"nine", 9},
    // for part one only
    
    {"1", 1},
    {"2", 2},
    {"3", 3},
    {"4", 4},
    {"5", 5},
    {"6", 6},
    {"7", 7},
    {"8", 8},
    {"9", 9}
  };



  /// gets the two digit number formed by the concatenation of the
  ///   **values** of the first and last occurrance of any substring
  ///   that is in `lookup`. check the examples.
  ///     
  function<short(string&)> get_number = [&](string &s){
    auto previous_value = optional<int>{};

    int number = 0;
    for(int i = 0; i < s.size(); i++)
      for(auto &[candidate, value] : lookup)

        if(i+candidate.size() <= s.size() && s.substr(i, candidate.size()) == candidate){

          if(!previous_value.has_value()) number += 10*value;
          previous_value = value;
        }
      
    
    number += previous_value.value();
    return number;
  };

  auto run_tests = [&](){
    string test_str = "onetwo";
    assert(get_number(test_str) == 12);
    test_str = "oneeight";
    assert(get_number(test_str) == 18);
    test_str = "oneight";
    assert(get_number(test_str) == 18);
    test_str = "four54";
    assert(get_number(test_str) == 44);
    test_str = "siixseeix7sx";
    assert(get_number(test_str) == 77);
  };

  run_tests();

  int ans = 0;
  string s;

  while(cin >> s){
    ans += get_number(s);
  }

  cout << ans << '\n';
}
