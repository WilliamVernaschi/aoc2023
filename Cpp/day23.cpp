#include <bits/stdc++.h>

using namespace std;

struct Position{
  int row, col;

  Position() : row(0), col(0) {}
  Position(int row, int col) : row(row), col(col) {};

  auto operator+(const Position &other) const{
    return Position(row+other.row, col+other.col);
  }
  auto operator*(int scalar) const{
    return Position(row*scalar, col*scalar);
  }
  auto operator<(const Position &other) const{
    return tuple(row, col) < tuple(other.row, other.col);
  }
  auto operator==(const Position &other) const{
    return tuple(row, col) == tuple(other.row, other.col);
  }
};

int main(){
  vector<string> mp;
  string line;
  while(cin >> line){
    mp.push_back(line);
  }
  int n = mp.size(), m = mp[0].size();
  vector<vector<bool>> visited(n, vector<bool>(m));
  auto valid = [&](Position p){
    return p.row >= 0 && p.col >= 0 && p.row < n && p.col < m;
  };

  int total = 0;
  int ans = 0;
  function<void(Position)> rec_p1 = [&](Position p){
    if(!valid(p)) return;
    if(visited[p.row][p.col]) return;
    char symbol = mp[p.row][p.col];
    if(symbol == '#') return;

    visited[p.row][p.col] = true;
    total++;
    if(p == Position(n-1, m-2)){
      ans = max(ans, total);
      cout << total-1 << '\n';
    }
    if(symbol == '>' || symbol == '.'){
      rec_p1(p + Position(0, 1));
    }
    if(symbol == '<' || symbol == '.'){
      rec_p1(p + Position(0, -1));
    }
    if(symbol == '^' || symbol == '.'){
      rec_p1(p + Position(-1, 0));
    }
    if(symbol == 'v' || symbol == '.'){
      rec_p1(p + Position(1, 0));
    }
    visited[p.row][p.col] = false;
    total--;
  };

  
  auto there_is_path = [&](Position from, Position to){
    vector<vector<bool>> local_visited(n, vector<bool>(m, false));

    function<bool(Position,Position)> there_is_path_rec = [&](Position from, Position to){
      
      if(from == to) return true;
      if(!valid(from)) return false;
      if(visited[from.row][from.col]) return false;
      if(local_visited[from.row][from.col]) return false;
      local_visited[from.row][from.col] = true;
      char symbol = mp[from.row][from.col];
      if(symbol == '#') return false;
      return there_is_path_rec(from + Position(0, 1), to)
          || there_is_path_rec(from + Position(0, -1), to)
          || there_is_path_rec(from + Position(-1, 0), to)
          || there_is_path_rec(from + Position(1, 0), to);
    };
    return there_is_path_rec(from, to);
  };

  // answer is between 6711 and 6800
  function<void(Position)> rec_p2 = [&](Position p){
    if(!valid(p)) return;
    if(visited[p.row][p.col]) return;
    char symbol = mp[p.row][p.col];
    if(symbol == '#') return;

    //if(!there_is_path(p, Position(n-1, m-2))) return;

    visited[p.row][p.col] = true;
    total++;
    if(p == Position(n-1, m-2)){
      if(total > ans) cout << ans << '\n';
      ans = max(ans, total);
    }
    rec_p2(p + Position(0, -1));
    rec_p2(p + Position(1, 0));
    rec_p2(p + Position(0, 1));
    rec_p2(p + Position(-1, 0));
    visited[p.row][p.col] = false;
    total--;
  };
  rec_p1(Position(0, 1));
  cout << "Part 1: " << ans-1 << '\n';
  ans = 0, total = 0;
  rec_p2(Position(0, 1));
  cout << "Part 2: " << ans-1 << '\n';
}
