#include <bits/stdc++.h>
#include <cctype>

using namespace std;

struct Condition{
  char direction;
  int val;
  int type;
  string result;

  bool evaluate(vector<int> xmas){
    if(direction == '>') return xmas[type] > val;
    else if(direction == '<') return xmas[type] < val;
    else return true;
  }
};

// [l, r)
struct XmasRange{
  vector<pair<int,int>> x;

  pair<int,int> join(pair<int,int> l, pair<int,int> r) const{
    pair<int,int> ans;
    ans.first = max(l.first, r.first);
    ans.second = min(l.first, r.first);
    if(ans.second >= ans.first) return {0, 0};
    return ans;
  };
  XmasRange(int start, int end){
    x.resize(4);
    x[0].first = x[1].first = x[2].first = x[3].first = start;
    x[0].second = x[1].second = x[2].second = x[3].second = end+1;
  }
  void update(int type, int direction, int newval){
    if(direction == '>') x[type].first = newval+1;
    else if(direction == '<') x[type].second = newval;
  }
  long long total_possibilities(){
    long long ans = 1;
    for(auto [b,e] : x) ans *= (e-b);
    return ans;
  }
};

int main(){
  map<string, vector<Condition>> mp;

  function<bool(vector<int>, string)> accepts = [&](vector<int> xmas, string at){
    if(at == "A") return true;
    else if(at == "R") return false;

    for(auto cond : mp[at]){
      if(cond.evaluate(xmas)) return accepts(xmas, cond.result);
    }
    return false;
  };


  auto parse_line = [](string line){

    auto parse_cond = [](string line){
      Condition c;
      auto postp = line.find(":");
      if(postp == string::npos){
        c.result = line;
        c.direction = -1;
        return c;
      }
      switch (line[0]) {
        case 'x':
          c.type = 0;
          break;
        case 'm':
          c.type = 1;
          break;
        case 'a':
          c.type = 2;
          break;
        case 's':
          c.type = 3;
          break;
      }
      c.direction = line[1];
      c.result = line.substr(postp+1, string::npos);
      c.val = stoi(line.substr(2, postp-2));
      return c;
    };

    string id;
    int i = 0;
    for(; line[i] != '{'; i++){
      id += line[i];
    }
    i++;

    vector<Condition> ans;

    string  cond;
    while(i < line.size()){
      cond = "";
      for(; line[i] != ',' && line[i] != '}'; i++){
        cond += line[i];
      }
      ans.push_back(parse_cond(cond));
      i++;
    }
    return make_pair(id, ans);
  };


  string line;
  while(getline(cin, line)){
    if(line == "") break;
    pair<string, vector<Condition>> parsed = parse_line(line);
    mp[parsed.first] = parsed.second;
  }


  auto parse_xmas = [](string line){
    string num;
    vector<int> xmas;
    for(int i = 1; i < line.size(); i++){
      if(isdigit(line[i])){
        num += line[i];
      }
      else{
        if(num != "") xmas.push_back(stoi(num));
        num = "";
      }
    }
    return xmas;
  };

  long long part1 = 0;
  while(getline(cin, line)){
    vector<int> xmas = parse_xmas(line);
    if(accepts(xmas, "in")){
      part1 += accumulate(xmas.begin(), xmas.end(), 0ll);
    }
  }
  cout << "Part 1: " << part1 << '\n';

  long long part2 = 0;
  function<void(XmasRange, string)> rec = [&](XmasRange cur_range, string at){

    if(at == "A"){
      part2 += cur_range.total_possibilities();
      return;
    }
    else if(at == "R"){
      return;
    }

    for(auto neighbour : mp[at]){
      auto prev = cur_range;
      cur_range.update(neighbour.type, neighbour.direction, neighbour.val);
      rec(cur_range, neighbour.result);

      int upddir, updval=neighbour.val;
      if(neighbour.direction == '>') upddir = '<';
      else if(neighbour.direction == '<') upddir = '>';
      if(neighbour.direction == '<') updval--;
      else if(neighbour.direction == '>') updval++;

      cur_range = prev;
      cur_range.update(neighbour.type, upddir, updval);
    }
  };

  XmasRange init_range = XmasRange(1, 4000);
  rec(init_range, "in");

  cout << "Part 2: " << part2 << '\n';

}
