#include <bits/stdc++.h>

using namespace std;


// TODO
int get_matches(string &s){
  return 0;
}

int main(){
  string s;

  vector<int> pfx_vector(200);
  int pfx_sum = 1;

  pfx_vector[0] = 1;

  int idx = 0;
  int part2 = 0, part1 = 0;

  while(cin >> s){
    int matches = get_matches(s);
    part1 += (1 << max(0, (matches-1)));

    pfx_vector[idx + 1] += pfx_sum;
    pfx_vector[idx + matches + 1] -= pfx_sum;

    part2 += pfx_sum;
    pfx_sum += pfx_vector[idx + 1];
  }
  part2 += pfx_sum;
}
