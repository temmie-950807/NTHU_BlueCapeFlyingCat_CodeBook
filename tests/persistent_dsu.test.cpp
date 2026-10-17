// competitive-verifier: STANDALONE
#include "common.hpp"
#include <cassert> // GCC 16 的 bits/stdc++.h 不含 assert
#include "../codes/Data-Structure/Segment_Tree_Persistent.cpp"
#include "../codes/Data-Structure/Disjoint_Set_Persistent.cpp"

// 與每個版本的暴力快照比對，並檢查 init(n, q) 配置的節點數足夠
signed main() {
    mt19937_64 rng(7122);
    auto rnd = [&](int l, int r) { return (int)(rng() % (r - l + 1)) + l; };
    Persistent_Segment_Tree st;
    Persistent_Disjoint_Set dsu;
    for (int tc = 0; tc < 300; tc++) {
        int n = rnd(1, 40), q = rnd(0, 80);

        // 持久化線段樹：clone 後 update 新版本，也會直接 update 舊版本
        vector<vector<int>> snap(1, vector<int>(n));
        for (auto &x : snap[0]) x = rnd(-9, 9);
        st.init(n, q);
        st.build(snap[0], 0);
        for (int i = 0; i < q; i++) {
            int ver = rnd(0, snap.size() - 1);
            if (rng() % 2) st.clone_version(ver), snap.push_back(snap[ver]);
            else {
                int pos = rnd(0, n - 1), val = rnd(-9, 9);
                st.update_version(pos, val, ver), snap[ver][pos] = val;
            }
            assert(st.node_cnt <= (int)st.arr.size());
            for (int k = 0; k < 5; k++) {
                int v = rnd(0, snap.size() - 1), l = rnd(0, n), r = rnd(0, n);
                if (l > r) swap(l, r);
                if (l == r) continue;
                int sum = 0;
                for (int j = l; j < r; j++) sum += snap[v][j];
                assert(st.query_version(l, r, v).val == sum);
            }
        }

        // 持久化並查集：版本 i = 第 i 次 unite 之後
        vector<vector<int>> comp(1, vector<int>(n));
        iota(comp[0].begin(), comp[0].end(), 0);
        dsu.init(n, q);
        for (int i = 0; i < q; i++) {
            int a = rnd(0, n - 1), b = rnd(0, n - 1);
            auto c = comp.back();
            bool merged = c[a] != c[b];
            if (merged) {
                int from = c[b];
                for (auto &x : c) if (x == from) x = c[a];
            }
            comp.push_back(c);
            assert(dsu.unite(a, b) == merged);
            assert(dsu.arr.node_cnt <= (int)dsu.arr.arr.size());
            assert(dsu.sz.node_cnt <= (int)dsu.sz.arr.size());
        }
        for (int k = 0; k < 200; k++) {
            int v = rnd(0, q), a = rnd(0, n - 1), b = rnd(0, n - 1);
            assert((dsu.find(a, v) == dsu.find(b, v)) == (comp[v][a] == comp[v][b]));
        }
    }
    puts("OK");
}
