// competitive-verifier: STANDALONE
#include "common.hpp"
#include <cassert> // GCC 16 的 bits/stdc++.h 不含 assert
#include "../codes/Data-Structure/Segment_Tree_Li_Chao_Line.cpp"

// 全域直線取最小值，x 涵蓋整個 [0, MAX_V)，與暴力比對
signed main() {
    mt19937_64 rng(7122);
    auto rnd = [&](int l, int r) { return (int)(rng() % (r - l + 1)) + l; };
    LC_Segment_Tree st;
    vector<pair<int, int>> lines;
    for (int i = 0; i < 3000; i++) {
        int a = rnd(-1e6, 1e6), b = rnd(-1e12, 1e12);
        st.update({a, b}), lines.push_back({a, b});
        for (int x : {0LL, MAX_V - 1, rnd(0, MAX_V - 1)}) {
            int best = INF;
            for (auto [p, q] : lines) best = min(best, p * x + q);
            assert(st.query(x) == best);
        }
    }
    puts("OK");
}
