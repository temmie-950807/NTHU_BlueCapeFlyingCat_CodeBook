// competitive-verifier: STANDALONE
#include "common.hpp"
#include "../codes/Graph/EdgeBCC.cpp"

mt19937 rng(7122);
int rnd(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }

// 和暴力比：拿掉邊 i 後連通塊數變多 <=> i 是橋；邊雙 = 拿掉所有橋後的連通塊
signed main() {
    for (int it = 0; it < 5000; it++) {
        int n = rnd(1, 9), m = rnd(0, 14);
        vector<pair<int, int>> E(m);
        EdgeBCC g(n);
        for (auto &[u, v] : E) u = rnd(0, n - 1), v = rnd(0, n - 1), g.add_edge(u, v); // 含自環、重邊
        g.get_bcc();
        auto comp = [&](int skip, bool noBridge) {
            vector<int> c(n, -1);
            int k = 0;
            for (int s = 0; s < n; s++) if (c[s] < 0) {
                vector<int> st{s}; c[s] = k;
                while (!st.empty()) {
                    int x = st.back(); st.pop_back();
                    for (int i = 0; i < m; i++) {
                        if (i == skip || (noBridge && g.isBridge[i])) continue;
                        auto [a, b] = E[i];
                        for (int y : {a == x ? b : -1, b == x ? a : -1})
                            if (y >= 0 && c[y] < 0) c[y] = k, st.push_back(y);
                    }
                }
                k++;
            }
            return pair{k, c};
        };
        int base = comp(-1, false).first;
        vector<pair<int, int>> br;
        for (int i = 0; i < m; i++) {
            bool b = comp(i, false).first > base;
            assert(g.isBridge[i] == b);
            if (b) br.push_back(E[i]);
        }
        assert(g.bridge == br);
        auto [k, c] = comp(-1, true);
        assert(g.sz == k);
        int tot = 0;
        for (int i = 0; i < g.sz; i++) {
            tot += g.bcc[i].size();
            for (int v : g.bcc[i]) assert(g.bccId[v] == i && c[v] == c[g.bcc[i][0]]);
        }
        assert(tot == n);
    }
    return 0;
}
