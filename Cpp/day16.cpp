#include <bits/stdc++.h>

using namespace std;

enum Direction{U, D, R, L};


int main(){

  string s; 
  vector<string> mp;

  while(cin >> s){
    mp.push_back(s);
  }

  int n = mp.size(), m = mp[0].size();

  auto valid = [&](int i, int j){
    return i >= 0 && j >= 0 && i < n && j < m;
  };

  vector<pair<int,int>> deltas(4);

  deltas[U] = {-1, 0};
  deltas[D] = {1, 0};
  deltas[R] = {0, 1};
  deltas[L] = {0, -1};


  vector<vector<vector<bool>>> marked(n, vector<vector<bool>>(m, vector<bool>(4, false)));

  auto solve = [&](int si, int sj, Direction sd){

    function<void(int,int,Direction)> dfs = [&](int i, int j, Direction d){
      if(!valid(i, j) || marked[i][j][d]) return 0;
      marked[i][j][d] = true;

      int ans = 1;

      char sitting = mp[i][j];
      bool empty = sitting == '.';
      bool straight_trough = (sitting == '-' && (d == L || d == R))
                          || (sitting == '|' && (d == U || d == D));

      if(empty || straight_trough){
        dfs(i + deltas[d].first, j + deltas[d].second, d);
      }
      else if((d == L || d == R) && sitting == '|'){
        dfs(i + deltas[U].first, j + deltas[U].second, U);
        dfs(i + deltas[D].first, j + deltas[D].second, D);
      }
      else if((d == U || d == D) && sitting == '-'){
        dfs(i + deltas[L].first, j + deltas[L].second, L);
        dfs(i + deltas[R].first, j + deltas[R].second, R);
      }
      else if(sitting == '\\'){
        Direction turn;
        if(d == R || d == D) turn = d == R ? D : R;
        else turn = d == U ? L : U;

        dfs(i + deltas[turn].first, j + deltas[turn].second, turn);
      }
      else{
        Direction turn;
        if(d == R || d == U) turn = d == R ? U : R;
        else turn = d == L ? D : L;

        dfs(i + deltas[turn].first, j + deltas[turn].second, turn);
      }

      return ans;
    };
    dfs(si, sj, sd);
    int ans = 0;
    for(int i = 0; i< n; i++){
      for(int j = 0; j < m; j++){
        if(marked[i][j][0] || marked[i][j][1] || marked[i][j][2] || marked[i][j][3])
          ans++;
        marked[i][j][0] = marked[i][j][1] = marked[i][j][2] = marked[i][j][3] = false;
      }
    }
    return ans;
  };


  
  cout << "Part 1: " << solve(0, 0, R) << '\n';

  int part2 = 0;

  for(int i = 0; i < n; i++)
    part2 = max(part2, solve(0, i, D)), part2 = max(part2, solve(n-1, i, U));
  for(int i = 0; i < m; i++)
    part2 = max(part2, solve(i, 0, R)), part2 = max(part2, solve(i, m-1, L));
  

  cout << "Part 2: " << part2 << '\n';


}

