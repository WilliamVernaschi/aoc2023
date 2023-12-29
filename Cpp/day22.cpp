#include <bits/stdc++.h>

using namespace std;


struct Coord{
  int x, y, z;
};

int main(){
  string line;

  auto parse_coords = [](string coord){
    int comma1 = coord.find(",");
    int comma2 = coord.find(",", comma1+1);

    Coord c;
    c.x = stoi(coord.substr(0, comma1));
    c.y = stoi(coord.substr(comma1+1, comma2-comma1));
    c.z = stoi(coord.substr(comma2+1, string::npos));

    return c;
  };

  vector<pair<Coord, Coord>> input;

  while(cin >> line){
    pair<Coord, Coord> coords;
    coords.first = parse_coords(line.substr(0, line.find("~")));
    coords.second = parse_coords(line.substr(line.find("~")+1, string::npos));


    input.push_back(coords);
  }
  sort(input.begin(), input.end(), [](pair<Coord, Coord> a, pair<Coord, Coord> b){
        return min(a.second.z, a.first.z) < min(b.second.z, b.first.z);
    });

  vector<vector<pair<int,int>>> xy(300, vector<pair<int,int>>(300, make_pair(-1, 0)));


  vector<vector<int>> supported_by(input.size());

  int idx = 0;
  for(auto [from, to] : input){
    vector<pair<int,int>> candidates;


    if(from.x > to.x || from.y > to.y || from.z > to.z) swap(from, to);
    
    if(from.x != to.x){
      int highest = 0;
      for(int i = from.x; i <= to.x; i++){
        highest = max(highest, xy[i][from.y].second);
      }
      for(int i = from.x; i <= to.x; i++){
        candidates.push_back(xy[i][from.y]);
        xy[i][from.y] = {idx, highest+1};
      }
    }
    else if(from.y != to.y){
      int highest = 0;
      for(int i = from.y; i <= to.y; i++){
        highest = max(highest, xy[from.x][i].second);
      }
      for(int i = from.y; i <= to.y; i++){
        candidates.push_back(xy[from.x][i]);
        xy[from.x][i] = {idx, highest+1};
      }
    }
    else{
      candidates.push_back(xy[from.x][from.y]);
      xy[from.x][from.y] = {idx, xy[from.x][from.y].second + to.z-from.z+1};
    }
    int highest_z = 0;
    for(auto [idx, height] : candidates) highest_z = max(height, highest_z);
    set<int> supporters;

    for(auto [sup_idx, height] : candidates){
      if(height == 0) continue;
      if(height == highest_z) supporters.insert(sup_idx);
      cout << '\n';
    }
    for(int supporter : supporters){
      supported_by[idx].push_back(supporter);
    }

    idx++;
  }

  int safe = input.size();
  set<int> unsafe;

  for(int i = 0; i < supported_by.size(); i++)
    if(supported_by[i].size() == 1)
      unsafe.insert(supported_by[i][0]);
    

  auto chain_reactions = [&](int block){
    vector<bool> removed(supported_by.size());
    removed[block] = true;

    function<int(int)> chain_reactions_rec = [&](int bk){
      int ans = 1;

      vector<int> to_remove;
      for(int i = 0; i < supported_by.size(); i++){
        int cnt_removed = 0;
        for(int sp : supported_by[i]) if(removed[sp]) cnt_removed++;

        if(cnt_removed == supported_by[i].size() && supported_by[i].size() > 0 && !removed[i]){
          to_remove.push_back(i);
        }
      }
      for(int rem : to_remove){
        removed[rem] = true;
      }
      for(int rem : to_remove){
        ans += chain_reactions_rec(rem);
      }
      return ans;
    };
    
    return chain_reactions_rec(block);
  };

  int part2 = 0;
  for(int x : unsafe){
    part2 += chain_reactions(x)-1;
    cout << x << " " << chain_reactions(x)-1 << '\n';
  }

  cout << "Part 1: " << input.size() - unsafe.size() << '\n';
  cout << "Part 2: " << part2 << '\n';

}



