// O(VE log(max_flow)) if scaling == 1
// O((V + E) sqrt(E)) if unit graph (turn scaling off)
// O((V + E) sqrt(V)) if bipartite matching (turn scaling off)
// indices are 0-based
template <typename T>
struct Dinic {
  struct Edge {
    int v, rev;
    T cap, flow;
  };

  int n;
  vector<vector<Edge>> g;
  vector<int> lvl, it;
  static constexpr T eps = (T)1e-9;
  bool scaling;

  Dinic(int n, bool scaling = true): n(n), g(n), lvl(n), it(n), scaling(scaling) {}

  void add_edge(int u, int v, T cap, bool directed = true) {
    Edge a{v, (int)g[v].size(), cap, 0};
    Edge b{u, (int)g[u].size(), directed ? 0 : cap, 0};
    g[u].push_back(a);
    g[v].push_back(b);
  }

  bool bfs(int s, int t, T lim) {
    fill(lvl.begin(), lvl.end(), -1);
    queue<int> q; q.push(s);
    lvl[s] = 0;
    while (!q.empty()) {
      int u = q.front(); q.pop();
      for (auto &e : g[u]) {
        if (lvl[e.v] == -1 && e.cap - e.flow >= lim + eps) {
          lvl[e.v] = lvl[u] + 1;
          q.push(e.v);
        }
      }
    }
    return lvl[t] != -1;
  }

  T dfs(int u, int t, T f) {
    if (u == t || f <= eps) return f;
    for (int &i = it[u]; i < (int)g[u].size(); i++) {
      Edge &e = g[u][i];
      if (lvl[e.v] == lvl[u] + 1 && e.cap - e.flow > eps) {
        T ret = dfs(e.v, t, min(f, e.cap - e.flow));
        if (ret > eps) {
          e.flow += ret;
          g[e.v][e.rev].flow -= ret;
          return ret;
        }
      }
    }
    return 0;
  }

  T max_flow(int s, int t) {
    T flow = 0;
    T lim = scaling ? (T)(1 << 30) : 1;
    while (lim > eps) {
      while (bfs(s, t, lim)) {
        fill(it.begin(), it.end(), 0);
        while (true) {
          T pushed = dfs(s, t, numeric_limits<T>::max());
          if (pushed <= eps) break;
          flow += pushed;
        }
      }
      lim /= 2;
    }
    return flow;
  }

  vector<int> min_cut(int s) {
    vector<int> vis(n, 0);
    queue<int> q; q.push(s); vis[s] = 1;
    while (!q.empty()) {
      int u = q.front(); q.pop();
      for (auto &e : g[u])
        if (!vis[e.v] && e.cap - e.flow > eps)
          vis[e.v] = 1, q.push(e.v);
    }
    return vis; // vis[i] == 1 -> S-side
  }

  vector<vector<int>> backtrack_paths(int s, int t) {
    vector<vector<int>> paths;
    vector<int> path;
    vector<int> used(n, 0);

    function<T(int, T)> dfs_path = [&](int u, T flow) -> T {
      if (u == t) {
        paths.push_back(path);
        return flow;
      }
      used[u] = 1;
      for (auto &e : g[u]) {
        if (!used[e.v] && e.flow > eps) {
          T pushed = min(flow, e.flow);
          path.push_back(e.v);
          T ret = dfs_path(e.v, pushed);
          e.flow -= ret;
          g[e.v][e.rev].flow += ret;
          path.pop_back();
        }
      }
      used[u] = 0;
      return 0;
    };

    path.push_back(s);
    dfs_path(s, numeric_limits<T>::max());
    return paths;
  }
};
