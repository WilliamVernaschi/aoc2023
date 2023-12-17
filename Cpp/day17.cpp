#include <bits/stdc++.h>

using namespace std;

enum Direction{U, D, L, R};

struct Node{
  Direction came_from;
  int row, col;

  Node(Direction d, int row, int col) : came_from(d), row(row), col(col) {};
  Node(int d, int row, int col) : came_from((Direction)d), row(row), col(col) {};

  auto operator<(const Node& other) const{
    return tuple(row, col, came_from) < tuple(other.row, other.col, other.came_from);
  };
  auto operator==(const Node& other) const{
    return tuple(row, col, came_from) == tuple(other.row, other.col, other.came_from);
  };
};
struct NodeHash {
  auto operator()(const Node &n) const -> size_t {
    return (((n.row + n.col)*(n.row + n.col + 1)/2) + n.col) | (1 << (n.came_from+25));
  }
};


int main(){

  vector<string> mp;


  string line;
  while(cin >> line){
    mp.push_back(line);
  }

  int n = mp.size(), m = mp[0].size();

  auto valid = [&](int x, int y) -> bool{
    return x >= 0 && y >= 0 && x < n && y < m;
  };

  auto add_edge = [&](Node a, Node b, unordered_map<Node, vector<pair<Node, int>>, NodeHash> &g) -> bool{
    if(valid(a.row, a.col) && valid(b.row, b.col)){
      int weight = 0;

      if(a.row == b.row){
        if(b.col > a.col){
          for(int i = a.col+1; i <= b.col; i++)
            weight += (mp[a.row][i] - '0');
        }
        else{
          for(int i = a.col-1; i >= b.col; i--)
            weight += (mp[a.row][i] - '0');
        }
      }
      else{
        if(b.row > a.row){
          for(int i = a.row+1; i <= b.row; i++)
            weight += (mp[i][a.col] - '0');
        }
        else{
          for(int i = a.row-1; i >= b.row; i--)
            weight += (mp[i][a.col] - '0');
        }
      }
      g[a].push_back({b, weight});
      return true;
    }
    return false;
  };

  auto build_graph = [&](int mn, int mx) -> unordered_map<Node, vector<pair<Node, int>>, NodeHash>{
    unordered_map<Node, vector<pair<Node, int>>, NodeHash> g;
    g.reserve((mx-mn)*n*m*2);

    for(int i = 0; i < n; i++){
      for(int j = 0; j < m; j++){
        for(int from = 0; from < 4; from++){
          for(int to = 0; to < 4; to++){
            if(from == to) continue;
            if(from == U && to == D) continue;
            if(from == D && to == U) continue;
            if(from == L && to == R) continue;
            if(from == R && to == L) continue;
            for(int shift = mn; shift <= mx; shift++){
              if(to == R){
                add_edge(Node(from, i, j), Node(to, i, j+shift), g);
              }
              else if(to == L){
                add_edge(Node(from, i, j), Node(to, i, j-shift), g);
              }
              else if(to == D){
                add_edge(Node(from, i, j), Node(to, i+shift, j), g);
              }
              else if(to == U){
                add_edge(Node(from, i, j), Node(to, i-shift, j), g);
              }
            }
          }
        }
      }
    }
    return g;
  };


  const int INF = 1e9;

  auto dijkstra = [&](int si, int sj, unordered_map<Node, vector<pair<Node,int>>, NodeHash> graph){
    unordered_map<Node, int, NodeHash> dist;


    set<pair<int, Node>> pq;
    pq.insert({0, Node(U, si, sj)});
    pq.insert({0, Node(L, si, sj)});
    dist[Node(U, si, sj)] = 0;
    dist[Node(L, si, sj)] = 0;

    while(!pq.empty()){
      auto cur_node = *pq.begin();
      pq.erase(pq.begin());
      Node v = cur_node.second;

      for(const auto &[u, edge] : graph[v]){
        if(!dist.count(u) || dist[v] + edge < dist[u]){
          pq.erase({dist[u], u});
          dist[u] = dist[v] + edge;
          pq.insert({dist[u], u});
        }
      }
    }

    return min({
        dist[Node(R, n-1, m-1)],
        dist[Node(D, n-1, m-1)]});
  };


  cout << "Part 1: " << dijkstra(0, 0, build_graph(1, 3)) << '\n';
  cout << "Part 2: " << dijkstra(0, 0, build_graph(4, 10)) << '\n';
}
