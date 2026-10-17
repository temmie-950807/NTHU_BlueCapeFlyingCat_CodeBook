// competitive-verifier: STANDALONE
#include "common.hpp"
#include "../codes/Graph/HLD.cpp" // 自帶 #include 與全域 G[N]，path_query 的 query 是留空的模板

mt19937 rng(7122);
int rnd(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }

// 檢查剖分的性質，並照 path_query 的迴圈取出區間，和暴力路徑比
signed main() {
    for (int it = 0; it < 3000; it++) {
        int n = it < 10 ? 2000 : rnd(1, 40);
        for (int i = 1; i <= n; i++) G[i].clear();
        vector<int> par(n + 1, 0);
        for (int i = 2; i <= n; i++) // 鏈、深樹、隨機樹
            par[i] = it % 3 == 0 ? i - 1 : it % 3 == 1 ? rnd(max(1ll, i - 3), i - 1) : rnd(1, i - 1);
        vector<int> lab(n + 1); // 重新編號，1 是根
        iota(lab.begin(), lab.end(), 0);
        shuffle(lab.begin() + 2, lab.end(), rng);
        vector<int> P(n + 1, -1);
        for (int i = 2; i <= n; i++) {
            int a = lab[i], b = lab[par[i]];
            G[a].push_back(b), G[b].push_back(a), P[a] = b;
        }
        HLD h(n);
        h.dfs1(), h.dfs2();
        vector<int> who(n + 1, 0);
        for (int v = 1; v <= n; v++) {
            assert(1 <= h.id[v] && h.id[v] <= n && !who[h.id[v]]);
            who[h.id[v]] = v;
            assert(h.pa[v] == P[v]);
        }
        for (int v = 1; v <= n; v++) {
            int best = 0;
            for (int u : G[v]) if (u != P[v]) best = max(best, h.sz[u]);
            assert(h.mxson[v] ? h.sz[h.mxson[v]] == best : best == 0);
            if (h.mxson[v]) assert(h.id[h.mxson[v]] == h.id[v] + 1 && h.topf[h.mxson[v]] == h.topf[v]);
            if (P[v] > 0 && h.mxson[P[v]] != v) assert(h.topf[v] == v);
            for (int u = v; u > 0; u = P[u]) // v 在 u 的子樹 <=> id 落在 [id[u], id[u] + sz[u])
                assert(h.id[u] <= h.id[v] && h.id[v] < h.id[u] + h.sz[u]);
        }
        assert(h.path_query(1, n) == 0);
        for (int q = 0; q < 30; q++) {
            int a = rnd(1, n), b = rnd(1, n);
            set<int> want;
            { // 暴力路徑
                vector<int> anc;
                for (int u = a; u > 0; u = P[u]) anc.push_back(u);
                int u = b;
                while (find(anc.begin(), anc.end(), u) == anc.end()) want.insert(u), u = P[u];
                for (int x : anc) { want.insert(x); if (x == u) break; }
            }
            vector<pair<int, int>> seg; // 照抄 path_query 的迴圈
            int x = a, y = b;
            while (h.topf[x] != h.topf[y]) {
                if (h.depth[h.topf[x]] < h.depth[h.topf[y]]) swap(x, y);
                seg.push_back({h.id[h.topf[x]], h.id[x]});
                x = h.pa[h.topf[x]];
            }
            if (h.depth[x] < h.depth[y]) swap(x, y);
            seg.push_back({h.id[y], h.id[x]});
            multiset<int> got;
            for (auto [l, r] : seg) { assert(l <= r); for (int i = l; i <= r; i++) got.insert(who[i]); }
            assert(got == multiset<int>(want.begin(), want.end()));
            assert((int)seg.size() <= 2 * __lg(n) + 2);
        }
    }
    return 0;
}
