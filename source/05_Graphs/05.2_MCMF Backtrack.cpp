// ---[ Min Cost Max Flow (Dijkstra + potentials, with path extraction) ]---
template<class FlowT = int, class CostT = long long>
struct MCMF { // In case CE, MCMF<ll,ll> mcmf;
    static constexpr CostT INF = numeric_limits<CostT>::max() / 4;
    static constexpr CostT EPS = (CostT)1e-9;

    struct E {
        int to, rev;
        FlowT cap, flow;
        CostT cost;
    };

    int n, s, t;
    vector<vector<E>> g;
    vector<CostT> dist, pot;
    vector<int> pv, pe;

    MCMF(int N,int S,int T):n(N),s(S),t(T),
        g(N),dist(N),pot(N),pv(N),pe(N){}

    void addEdge(int u,int v,FlowT cap,CostT cost){
        E a{v,(int)g[v].size(),cap,0,cost};
        E b{u,(int)g[u].size(),0,0,-cost};
        g[u].push_back(a); g[v].push_back(b);
    }

    // optional: initialize potentials if negative costs exist
    bool initPot() {
        fill(pot.begin(), pot.end(), INF);
        pot[s] = 0;
        for (int it = 0; it < n - 1; ++it) {
            bool any = false;
            for (int u = 0; u < n; ++u) if (pot[u] < INF/2) {
                for (auto &e : g[u]) if (e.cap > 0)
                    if (pot[e.to] > pot[u] + e.cost) {
                        pot[e.to] = pot[u] + e.cost;
                        any = true;
                    }
            }
            if (!any) break;
        }
        // check negative cycle
        for (int u = 0; u < n; ++u) if (pot[u] < INF/2)
            for (auto &e : g[u]) if (e.cap > 0)
                if (pot[e.to] > pot[u] + e.cost) return false;
        return true;
    }

    bool dijkstra(){
        fill(dist.begin(), dist.end(), INF);
        priority_queue<pair<CostT,int>, vector<pair<CostT,int>>, greater<>> pq;
        dist[s]=0; pq.push({0,s});
        while(!pq.empty()){
            auto [d,u]=pq.top(); pq.pop();
            if(d > dist[u] + EPS) continue;
            for(int i=0;i<(int)g[u].size();i++){
                E &e=g[u][i];
                if(e.cap > e.flow){
                    CostT nd=d+e.cost+pot[u]-pot[e.to];
                    if(nd+EPS < dist[e.to]){
                        dist[e.to]=nd; pv[e.to]=u; pe[e.to]=i;
                        pq.push({nd,e.to});
                    }
                }
            }
        }
        return dist[t] < INF/2;
    }

    pair<FlowT,CostT> flow(){
        FlowT totF=0; CostT totC=0;
        while(dijkstra()){
            for(int i=0;i<n;i++) if(dist[i]<INF/2) pot[i]+=dist[i];
            FlowT add=numeric_limits<FlowT>::max();
            for(int v=t;v!=s;v=pv[v])
                add=min(add, g[pv[v]][pe[v]].cap - g[pv[v]][pe[v]].flow);
            for(int v=t;v!=s;v=pv[v]){
                E &e=g[pv[v]][pe[v]];
                e.flow+=add; g[v][e.rev].flow-=add;
                totC += (CostT)add * e.cost;
            }
            totF += add;
        }
        return {totF, totC};
    }

    // ---- backtrack flow decomposition ----
    FlowT dfsPath(int u, FlowT f, vector<int> &path, vector<vector<E>> &res) const {
        if(u == t) return f;
        for(auto &e: res[u]) if(e.flow > EPS){
            FlowT pushed = dfsPath(e.to, min(f, e.flow), path, res);
            if(pushed > EPS){
                e.flow -= pushed;
                path.push_back(u);
                return pushed;
            }
        }
        return 0;
    }

    vector<pair<vector<int>,FlowT>> getPaths() const {
        auto res = g;
        vector<pair<vector<int>,FlowT>> out;
        while(true){
            vector<int> path;
            FlowT pushed = dfsPath(s, numeric_limits<FlowT>::max(), path, res);
            if(pushed <= EPS) break;
            reverse(path.begin(), path.end());
            path.push_back(t);
            out.push_back({path, pushed});
        }
        return out;
    }
};
