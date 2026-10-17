// competitive-verifier: STANDALONE
#include "common.hpp"
#include "../codes/Data-Structure/Segment_Tree_Li_Chao_Segment.cpp"

// 線段 [ql, qr) 取最小值，與暴力比對（含端點 0 / N、長度 1 的線段、沒有線段時回傳 INF）
signed main() {
    mt19937_64 rng(7122);
    auto rnd = [&](int l, int r) { return (int)(rng() % (r - l + 1)) + l; };
    for (int it = 0; it < 300; it++) {
        int N = it < 10 ? it + 1 : it % 3 ? rnd(1, 80) : rnd(1, 1000000);
        int A = it % 2 ? 3 : 1000000, Bv = it % 2 ? 5 : 1000000000000LL;
        LC_Segment_Tree st(N);
        vector<array<int, 4>> segs;
        for (int op = 0; op < 150; op++) {
            if (rng() % 2) {
                int l = rnd(0, N - 1), r = rnd(l + 1, N);
                if (op % 7 == 0) l = 0, r = N;
                if (op % 11 == 0) r = l + 1;
                int a = rnd(-A, A), b = rnd(-Bv, Bv);
                st.update_segment({a, b}, l, r), segs.push_back({a, b, l, r});
            }
            for (int x : {0LL, N - 1, rnd(0, N - 1)}) {
                int best = INF;
                for (auto [a, b, l, r] : segs) if (l <= x && x < r) best = min(best, a * x + b);
                assert(st.query(x) == best);
            }
        }
    }
    puts("OK");
    return 0;
}
