#include <bits/stdc++.h>

using namespace std;


using grid = vector<string>;

int reflection_misses(int idx, const vector<int> &gd){
  int l = idx, r = idx+1;
  int misses = 0;

  while(l >= 0 && r < (int)gd.size()){
    misses += __builtin_popcount(gd[l]^gd[r]);
    l--, r++;
  }
  return misses;
};

grid transpose(const grid &gd){
  int n = (int)gd.size(), m = (int)gd[0].size();
  vector<string> trans(m, string(n, '.'));

  for(int i = 0; i < m; i++){
    for(int j = 0; j < n; j++){
      trans[i][j] = gd[j][i];
    }
  }

  return trans;
};

vector<int> grid_hash(const grid &g){
  vector<int> hsh;
  for(string line : g){
    reverse(line.begin(), line.end());
    hsh.push_back(0);
    for(int i = 0; i < (int)line.size(); i++){
      if(line[i] == '#') hsh.back() += (1 << i);
    }
  };
  return hsh;
};

int main(){
  string line;
  vector<grid> grids;

  grid g;
  while(getline(cin, line)){
    if(line == ""){
      grids.push_back(g);
      g.clear();
    }
    else{
      g.push_back(line);
    }
  }
  grids.push_back(g);


  int part1 = 0, part2 = 0;

  for(grid gd : grids){
    vector<int> horizontal_hash = grid_hash(gd);
    vector<int> vertical_hash = grid_hash(transpose(gd));

    int original_horizontal = 0, original_vertical = 0;

    for(int i = 0; i < (int)horizontal_hash.size()-1; i++){
      if(reflection_misses(i, horizontal_hash) == 0){
        original_horizontal = i+1;
        break;
      }
    }
    for(int i = 0; i < (int)vertical_hash.size()-1; i++){
      if(reflection_misses(i, vertical_hash) == 0){
        original_vertical = i+1;
        break;
      }
    }

    part1 += original_horizontal*100 + original_vertical;

    int modified_horizontal = 0, modified_vertical = 0;

    for(int i = 0; i < (int)horizontal_hash.size()-1; i++){
      if(reflection_misses(i, horizontal_hash) == 1){
        modified_horizontal = (i+1);
        break;
      }
    }
    for(int i = 0; i < (int)vertical_hash.size()-1; i++){
      if(reflection_misses(i, vertical_hash) == 1){
        modified_vertical = (i+1);
        break;
      }
    }

    part2 += modified_horizontal*100 + modified_vertical;
  }

  cout << "Part 1: " << part1 << '\n';
  cout << "Part 2: " << part2 << '\n';
}
