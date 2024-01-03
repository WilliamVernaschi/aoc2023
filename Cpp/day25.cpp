#include <bits/stdc++.h>

using namespace std;

using Graph = vector<vector<int>>;

int main(){
  Graph adj;
  int n;

  auto add_edge = [&](int u, int v) -> void{
    adj[u].push_back(v);
    adj[v].push_back(u);
  };
  auto remove_edge = [&](int u, int v) -> void{
    adj[u].erase(find(adj[u].begin(), adj[u].end(), v));
    adj[v].erase(find(adj[v].begin(), adj[v].end(), u));
  };
  auto count_component = [&](int v) -> int{
    vector<bool> vis(n);
    int ans = 0;

    function<void(int)> dfs = [&](int x){
      vis[x] = true, ans++;

      for(int u : adj[x]) if(!vis[u])
        dfs(u);
    };

    dfs(v);
    return ans;
  };

  vector<pair<int,int>> edges;

  auto receive_graph = [&]() -> void{

    n = 0;

    string u, v;
    cin >> u;
    u.pop_back();
    map<string, int> vertex_id;
    map<int, string> id_vertex;
    while(cin >> v){
      if(v.back() == ':'){
        v.pop_back();
        u = v;
        continue;
      }
      if(!vertex_id.count(u)) vertex_id[u] = n++, id_vertex[n-1] = u;
      if(!vertex_id.count(v)) vertex_id[v] = n++, id_vertex[n-1] = v;

      edges.emplace_back(vertex_id[u], vertex_id[v]);
    }
    n = (int)vertex_id.size();
    adj.resize(n);

    for(auto [u, v] : edges){
      add_edge(u, v);
      cout << u << " " << v << '\n';
    }
  };

  auto compute_min_distances = [&]() -> vector<vector<int>>{

    vector<vector<int>> dist(n, vector<int>(n, INT_MAX));

    auto bfs = [&](int start){
      queue<int> q;
      q.push(start);
      dist[start][start] = 0;
      while(!q.empty()){
        int u = q.front(); q.pop();
        for(int v : adj[u]){
          if(dist[start][v] != INT_MAX) continue;
          dist[start][v] = dist[start][u] + 1;
          q.push(v);
        }
      }
    };

    for(int i = 0; i < n; i++) bfs(i);
    return dist;
  };

  receive_graph();
  vector<vector<int>> dist = compute_min_distances();


  std::random_device rd;  
  std::mt19937 gen(rd()); 

  auto can_be_bridge = [&](int u, int v){
    int tries = 300, failures = 0;
    for(int i = 0; i < tries; i++){
      int random_vertex = uniform_int_distribution<>(0, n-1)(gen);
      if(abs(dist[u][random_vertex] - dist[v][random_vertex]) == 0) failures++;
    }
    return failures <= 0.05*tries;
  };

  auto solve_p1 = [&](){
    for(int i = 0; i < (int)edges.size(); i++){

      auto [u1, v1] = edges[i];
      if(!can_be_bridge(u1, v1)) continue;
      remove_edge(u1, v1);

      for(int j = i+1; j < (int)edges.size(); j++){
        auto [u2, v2] = edges[j];
        if(!can_be_bridge(u2, v2)) continue;
        remove_edge(u2, v2);

        for(int k = j+1; k < (int)edges.size(); k++){
          auto [u3, v3] = edges[k];
          if(!can_be_bridge(u3, v3)) continue;
          remove_edge(u3, v3);

          int comp = count_component(0);
          if(comp < n){
            return comp * (n-comp);
          }
          add_edge(u3, v3);
        }
        add_edge(u2, v2);
      }
      add_edge(u1, v1);
    }
    assert(false);
    return -1;
  };

  cout << "Part 1: " << solve_p1() << '\n';


}
