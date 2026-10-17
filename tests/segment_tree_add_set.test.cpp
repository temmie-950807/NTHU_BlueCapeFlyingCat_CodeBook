// competitive-verifier: STANDALONE
#include "common.hpp"
#include <cassert> // GCC 16 的 bits/stdc++.h 不含 assert
#include "../codes/Data-Structure/Segment_Tree_Add_Set.cpp"

// 與暴力比對：含賦值為 0、全負數、空區間、n = 1、同一個 ST 重複 init
signed main() {
    mt19937_64 rng(7122);
    auto rnd = [&](int l, int r) { return (int)(rng() % (r - l + 1)) + l; };
    for (int tc = 0; tc < 3000; tc++) {
        int n = rnd(1, 12);
        vector<int> a(n);
        for (auto &x : a) x = rnd(-5, tc % 2 ? -1 : 5);
        ST.init(n);
        ST.build(a);
        for (int op = 0; op < 60; op++) {
            int l = rnd(0, n), r = rnd(0, n);
            if (l > r) swap(l, r);
            int t = rnd(0, 2), v = rnd(-3, 3);
            if (t == 0) {
                ST.add(l, r, v);
                for (int i = l; i < r; i++) a[i] += v;
            } else if (t == 1) {
                ST.set(l, r, v);
                for (int i = l; i < r; i++) a[i] = v;
            } else {
                auto res = ST.query(l, r);
                int sum = 0, ma = -INF;
                for (int i = l; i < r; i++) sum += a[i], ma = max(ma, a[i]);
                assert(res.sum == sum && res.ma == ma);
            }
        }
    }
    puts("OK");
}
