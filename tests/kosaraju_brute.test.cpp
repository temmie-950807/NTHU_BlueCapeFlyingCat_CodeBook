// competitive-verifier: STANDALONE
#include "common.hpp"
#include "../codes/Graph/Kosaraju_SCC.cpp"

mt19937 rng(7122);
int rnd(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }

// 和遞移閉包比：同一個 SCC <=> 互相可達；縮點圖的邊集合、SCC 編號是拓撲序
signed main() {
    for (int it = 0; it < 5000; it++) {
        int n = rnd(1, 10), m = rnd(0, 25);
        SCC_compress g(n);
        vector<vector<int>> R(n, vector<int>(n));
        for (int i = 0; i < n; i++) R[i][i] = 1;
        vector<pair<int, int>> E(m);
        for (auto &[u, v] : E) u = rnd(0, n - 1), v = rnd(0, n - 1), g.add_edge(u, v), R[u][v] = 1;
        g.compress();
        for (int k = 0; k < n; k++) for (int i = 0; i < n; i++) for (int j = 0; j < n; j++)
            R[i][j] |= R[i][k] & R[k][j];
        assert(g.sz == (int)g.SCC.size());
        int tot = 0;
        for (int i = 0; i < g.sz; i++) {
            tot += g.SCC[i].size();
            for (int v : g.SCC[i]) assert(g.SCC_id[v] == i);
        }
        assert(tot == n);
        for (int i = 0; i < n; i++) for (int j = 0; j < n; j++)
            assert((g.SCC_id[i] == g.SCC_id[j]) == (R[i][j] && R[j][i]));
        set<pair<int, int>> want;
        for (auto [u, v] : E) if (g.SCC_id[u] != g.SCC_id[v]) want.insert({g.SCC_id[u], g.SCC_id[v]});
        set<pair<int, int>> got;
        for (int i = 0; i < g.sz; i++) {
            auto adj = g[i];
            assert(is_sorted(adj.begin(), adj.end()) && adjacent_find(adj.begin(), adj.end()) == adj.end());
            for (int j : adj) got.insert({i, j}), assert(i < j); // 拓撲序
        }
        assert(got == want);
    }
    return 0;
}
