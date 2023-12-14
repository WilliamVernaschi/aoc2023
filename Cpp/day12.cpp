#include <bits/stdc++.h>

using namespace std;

using ll =long long;

ll memo[200][50][50];

vector<string> split(const string& str, char split_char) {
  const char* cur = str.c_str();
  const char* str_end = str.c_str() + str.size();

  vector<string> chunks;
  while (cur < str_end)
  {
    const char* start = cur;
    while (start < str_end and *start == split_char)
        start++;

    const char* end = start + 1;
    while (end < str_end and *end != split_char)
        end++;

    cur = end + 1;
    chunks.push_back(string(start, end - start));
  }
  return chunks;
}

int main(){
  string spring, groups_s;

  auto unfold = [](string s){
    string ans = "";
    for(int i = 0; i < 5; i++) ans += s + "?";
    ans.pop_back();
    return ans;
  };

  auto unfoldg = [](vector<int> g){
    vector<int> ans;
    for(int i = 0; i < 5; i++){
      for(int k : g) ans.push_back(k);
    }
    return ans;
  };


  ll part1 = 0, part2 = 0;

  while(cin >> spring >> groups_s){

    vector<string> groups_sp = split(groups_s, ',');
    vector<int> groups(groups_sp.size());
    for(int i = 0; i < groups_sp.size(); i++){
      groups[i] = stoi(groups_sp[i]);
    }

    function<ll(int,int,int)> rec = [&](int idx, int sz, int gp){
      ll &ans = memo[idx][sz][gp];

      if(ans != -1) return ans;

      if(gp < groups.size() && sz > groups[gp]) return 0ll;
      if(gp == groups.size() && sz) return 0ll;
      if(idx == spring.size()-1){
        if((sz == 0 && gp == groups.size()) || (gp == groups.size()-1 && sz == groups.back())) return 1ll;
        else return 0ll;
      }
      else{
        ans = 0;
        if(spring[idx+1] == '#'){
          ans = rec(idx+1, sz+1, gp);
        }
        else if(spring[idx+1] == '?'){
          ans += rec(idx+1, sz+1, gp);
          if(gp < groups.size() && sz == groups[gp]){
            ans += rec(idx+1, 0, gp+1);
          }
          else if(sz == 0){
            ans += rec(idx+1, 0, gp);
          }
        }
        else{
          if(gp < groups.size() && sz == groups[gp]){
            ans = rec(idx+1, 0, gp+1);
          }
          else if(sz == 0){
            ans = rec(idx+1, 0, gp);
          }
        }
        return ans;
      }
    };

    spring = "x" + spring;

    memset(memo, -1, sizeof(memo));
    part1 += rec(0, 0, 0);

    spring = spring.substr(1, string::npos);

    spring = unfold(spring);
    groups = unfoldg(groups);

    spring = "x" + spring;

    memset(memo, -1, sizeof(memo));
    part2 += rec(0, 0, 0);
  }
  cout << "Part 1: " << part1 << '\n';
  cout << "Part 2: " << part2 << '\n';
}
