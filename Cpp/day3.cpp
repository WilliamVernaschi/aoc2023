#include <cctype>
#include <map>
#include <set>
#include <vector>
#include <iostream>

using namespace std;

const int DX[] = {1, 1, 1, -1, -1, -1, 0, 0};
const int DY[] = {1, -1, 0, 1, -1, 0, 1, -1};

int main(){

  string s;
  vector<string> grid;

  while(cin >> s){
    grid.push_back(s);
  }

  map<pair<int,int>, vector<int>> symbol_position_to_adjacent_numbers;
  set<pair<int,int>> adjacent_symbols;

  auto issymbol = [](char c){
    return !isdigit(c) && c != '.';
  };

  auto valid = [&](int x, int y){
    return x >= 0 && y >= 0 && x < grid.size() && y < grid[0].size();
  };

  int part1 = 0;

  for(int i = 0; i < grid.size(); i++){
    string num;

    for(int j = 0; j < grid[i].size(); j++){
      if(isdigit(grid[i][j])) num += grid[i][j];

      // iterates trough neighbours
      for(int k = 0; k < 8; k++){
        if(!valid(i + DX[k], j + DY[k])) continue;

        // maintains a set of all gears adjacent to the current number
        if(issymbol(grid[i + DX[k]][j + DY[k]]) && isdigit(grid[i][j])){
          adjacent_symbols.emplace(i + DX[k], j + DY[k]);
        }
      }

      if(j == grid[i].size()-1 || !isdigit(grid[i][j])){
        if(num.empty()) continue;

        if(!adjacent_symbols.empty()) 
          part1 += stoi(num);

        // assigns adjacent the number found to every gear adjacent to it
        for(auto symbol_position : adjacent_symbols)
          symbol_position_to_adjacent_numbers[symbol_position].push_back(stoi(num));
        
        adjacent_symbols.clear();
        num = "";
      }
    }
  }

  int part2 = 0;
  
  for(auto &[symbol_position, adjacent_numbers] : symbol_position_to_adjacent_numbers){

    int i = symbol_position.first, j = symbol_position.second;
    if(grid[i][j] != '*') continue;


    if(adjacent_numbers.size() == 2){
      part2 += adjacent_numbers[0] * adjacent_numbers[1];
    }
  }

  cout << part1 << '\n';
  cout << part2 << '\n';

}
