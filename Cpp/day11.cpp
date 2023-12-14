#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

bool empty_row(string s){
  for(auto c : s) if(c != '.') return false;
  return true;
};

long long solve(const vector<string> &mp, const int expansion_constant){
  int n = mp.size(), m = mp[0].size();

  vector<long long> weight_row_pfx(n+1), weight_col_pfx(m+1);

  for(int i = 0; i < mp.size(); i++){
    weight_row_pfx[i+1] = weight_row_pfx[i];
    if(empty_row(mp[i])) weight_row_pfx[i+1] += expansion_constant;
    else weight_row_pfx[i+1]++;
  }

  vector<pair<int,int>> galaxies;

  for(int i = 0; i < m; i++){
    bool empty = true;
    for(int j = 0; j < n; j++){
      if(mp[j][i] != '.'){
        empty = false;
      }
      if(mp[j][i] == '#'){
        galaxies.push_back({j, i});
      }
    }
    weight_col_pfx[i+1] = weight_col_pfx[i];
    if(empty) weight_col_pfx[i+1] += expansion_constant;
    else weight_col_pfx[i+1]++;

  }

  long long ans = 0;
  for(auto [a,b] : galaxies){
    for(auto [x,y] : galaxies){

      long long vertical =
        a != x ? weight_row_pfx[max(a,x)] - weight_row_pfx[min(a,x)+1] + 1
               : 0;
      long long horizontal =
        b != y ? weight_col_pfx[max(b,y)] - weight_col_pfx[min(b,y)+1] + 1
               : 0;

      ans += vertical + horizontal;
    }
  }
  return ans/2;

}


int main(){
  string in; 

  vector<string> mp;

  while(cin >> in){
    mp.push_back(in);
  }

  cout << "Part 1: " << solve(mp, 2) << '\n';
  cout << "Part 2: " << solve(mp, 1e6) << '\n';



}
