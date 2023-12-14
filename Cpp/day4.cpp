#include <bits/stdc++.h>

using namespace std;


vector<int> string_to_nums(string s){

  vector<int> nums;
  string n;

  for(int i = 0; i < (int)s.size(); i++){
    if(isspace(s[i]) && !n.empty())
      nums.push_back(stoi(n)), n = "";
    else if(isdigit(s[i]))
      n += s[i];
  }

  if(!n.empty()) nums.push_back(stoi(n));
  return nums;
};

int get_matches(string &s){
  int tp = s.find(':')+1;
  int pipe = s.find("|");
  vector<int> set1 = string_to_nums(s.substr(tp, pipe-tp));
  vector<int> set2 = string_to_nums(s.substr(pipe+1, string::npos));

  int matches = 0;
  for(int el : set1){
    if(find(set2.begin(), set2.end(), el) != set2.end()) matches++;
  }
  return matches;
}

int main(){
  string s;

  unordered_map<int,int> pfx_map;
  int pfx_sum = 1;

  pfx_map[0] = 1;

  int part1 = 0, part2 = 0, idx = 0;

  // runs in linear time
  //
  // pfx_map[0] + pfx_map[1] + ... + pfx_map[k] = number of cards k-1.
  // at every iteration in this loop, pfx_sum contains the number of cards of the current card
  // being processed.
  while(getline(cin, s)){
    int matches = get_matches(s);

    if(matches-1 >= 0)
      part1 += (1 << (matches-1));

    pfx_map[idx + 1] += pfx_sum;
    pfx_map[idx + matches + 1] -= pfx_sum;

    part2 += pfx_sum;
    pfx_sum += pfx_map[idx + 1];
    idx++;
  }

  cout << "Part 1: " << part1 << '\n';
  cout << "Part 2: " << part2 << '\n';
}
