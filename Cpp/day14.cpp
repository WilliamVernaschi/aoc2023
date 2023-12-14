#include <bits/stdc++.h>

using namespace std;

using grid = vector<string>;

enum direction {N, S, E, W};

int main(){

  string line; 
  grid g;
  while(cin >> line){
    g.push_back(line);
  }
  int n = (int)g.size(), m = (int)g[0].size();

  vector<int> fall_on(m, -1);
  map<grid, int> cycles;

  int part1 = 0;

  for(int i = 0; i < n; i++){
    for(int j = 0; j < m; j++){
      if(g[i][j] == 'O'){
        fall_on[j]++;
        part1 += n - fall_on[j];
      }
      else if(g[i][j] == '#'){
        fall_on[j] = i;
      }
    }
  }

  auto rotateN = [](grid &g){
    int n = g.size(), m = g[0].size();
    vector<int> fall_on(m, -1);

    for(int i = 0; i < n; i++){
      for(int j = 0; j < m; j++){
        if(g[i][j] == 'O'){
          fall_on[j]++;

          g[i][j] = '.';
          g[fall_on[j]][j] = 'O';
        }
        else if(g[i][j] == '#'){
          fall_on[j] = i;
        }
      }
    }

  };

  auto rotateS = [](grid &g){
    int n = g.size(), m = g[0].size();
    vector<int> fall_on(m, n);

    for(int i = n-1; i >= 0; i--){
      for(int j = 0; j < m; j++){
        if(g[i][j] == 'O'){
          fall_on[j]--;

          g[i][j] = '.';
          g[fall_on[j]][j] = 'O';
        }
        else if(g[i][j] == '#'){
          fall_on[j] = i;
        }
      }
    }
  };

  auto rotateW = [](grid &g){
    int n = g.size(), m = g[0].size();
    vector<int> fall_on(n, -1);

    for(int j = 0; j < m; j++){
      for(int i = 0; i < n; i++){
        if(g[i][j] == 'O'){
          fall_on[i]++;

          g[i][j] = '.';
          g[i][fall_on[i]] = 'O';
        }
        else if(g[i][j] == '#'){
          fall_on[i] = j;
        }
      }
    }
  };

  auto rotateE = [](grid &g){
    int n = g.size(), m = g[0].size();
    vector<int> fall_on(n, m);


    for(int j = m-1; j >= 0; j--){
      for(int i = 0; i < n; i++){
        if(g[i][j] == 'O'){
          fall_on[i]--;

          g[i][j] = '.';
          g[i][fall_on[i]] = 'O';
        }
        else if(g[i][j] == '#'){
          fall_on[i] = j;
        }
      }
    }
  };

  const int ROTATIONS = 1000000000;

  int idx = 0;
  while(!cycles.count(g)){
    cycles[g] = idx++;
    rotateN(g);
    rotateW(g);
    rotateS(g);
    rotateE(g);
  }

  int at = idx;
  int length = (idx - cycles[g]);

  int start = at + length*((ROTATIONS - at)/length);

  for(int k = start; k < ROTATIONS; k++){
    rotateN(g);
    rotateW(g);
    rotateS(g);
    rotateE(g);
  }

  int part2 = 0;

  for(int i = 0; i < n; i++){
    for(int j = 0; j < m; j++){
      if(g[i][j] == 'O'){
        part2 += n - i;
      }
    }
  }


  
  cout << "Part 1: " << part1 << '\n';
  cout << "Part 2: " << part2 << '\n';


}
