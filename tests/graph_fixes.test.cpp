// competitive-verifier: STANDALONE
#include <cassert>
#include "common.hpp"
#include "../codes/Graph/2-SAT.cpp"
#include "../codes/Graph/Augment_Path.cpp"
#include "../codes/Graph/圓方樹.cpp"
#include "../codes/Graph/C3C4.cpp"
#include "../codes/Graph/Dinic.cpp"
#include "../codes/Graph/最大權閉合圖.cpp"
#include "../codes/Graph/MCMF.cpp"

mt19937 rng(7122);
int rnd(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }

// 2-SAT：同一個物件重複 init（先大後小），和暴力比
void test_two_sat() {
    TWO_SAT ts;
    for (int it = 0; it < 2000; it++) {
        int n = (it % 2 == 0) ? 8 : rnd(1, 5), m = rnd(1, 3 * n);
        vector<array<int, 4>> C(m);
        ts.init(n);
        for (auto &[a, na, b, nb] : C) {
            a = rnd(0, n - 1), na = rnd(0, 1), b = rnd(0, n - 1), nb = rnd(0, 1);
            ts.add_disjunction(a, na, b, nb);
        }
        auto ok = [&](auto val) { // val(i) = x_i
            for (auto [a, na, b, nb] : C)
                if (val(a) == na && val(b) == nb) return false;
            return true;
        };
        bool brute = false;
        for (int s = 0; s < (1 << n) && !brute; s++) brute = ok([&](int i) { return s >> i & 1; });
        assert(ts.solve() == brute);
        if (brute) {
            vector<int> res;
            ts.get_result(res);
            assert((int)res.size() == n && ok([&](int i) { return res[i]; }));
        }
    }
}

// Augment_Path：m > n 時最小點覆蓋（會用到 visy[0..m)）
void test_augment_path() {
    for (int it = 0; it < 2000; it++) {
        int n = rnd(1, 5), m = rnd(n, 9);
        AugmentPath g(n, m);
        vector<pair<int, int>> E;
        for (int x = 0; x < n; x++) for (int y = 0; y < m; y++)
            if (rnd(0, 2) == 0) E.push_back({x, y}), g.add(x, y);
        int best = 0; // dp[i][mask]：左邊前 i 個點、右邊用掉 mask
        vector<vector<int>> dp(n + 1, vector<int>(1 << m, -1));
        dp[0][0] = 0;
        for (int i = 0; i < n; i++) for (int s = 0; s < (1 << m); s++) if (dp[i][s] >= 0) {
            dp[i + 1][s] = max(dp[i + 1][s], dp[i][s]);
            for (int y : g.G[i]) if (!(s >> y & 1))
                dp[i + 1][s | 1 << y] = max(dp[i + 1][s | 1 << y], dp[i][s] + 1);
        }
        for (int s = 0; s < (1 << m); s++) best = max(best, dp[n][s]);
        auto mt = g.find_max_matching();
        assert((int)mt.size() == best);
        set<int> ux, uy;
        for (auto [x, y] : mt) assert(count(E.begin(), E.end(), make_pair(x, y))), ux.insert(x), uy.insert(y);
        assert((int)ux.size() == best && (int)uy.size() == best);
        auto vc = g.find_min_vertex_cover();
        assert((int)vc.size() == best);
        vector<int> cx(n), cy(m);
        for (auto [s, i] : vc) (s == 1 ? cx : cy)[i] = 1;
        for (auto [x, y] : E) assert(cx[x] || cy[y]);
    }
}

int components(int n, vector<pair<int, int>> &E, int del) {
    vector<int> p(n);
    iota(p.begin(), p.end(), 0);
    function<int(int)> f = [&](int x) { return p[x] == x ? x : p[x] = f(p[x]); };
    int c = n - (del >= 0);
    for (auto [u, v] : E) if (u != del && v != del && f(u) != f(v)) p[f(u)] = f(v), c--;
    return c;
}

// 圓方樹 is_cut：含孤點的簡單圖，和「拔掉後連通塊變多」比
void test_is_cut() {
    for (int it = 0; it < 2000; it++) {
        int n = rnd(1, 8);
        BCC_AP g(n);
        vector<pair<int, int>> E;
        for (int u = 0; u < n; u++) for (int v = u + 1; v < n; v++)
            if (rnd(0, 3) == 0) E.push_back({u, v}), g.add_edge(u, v);
        g.build();
        int base = components(n, E, -1);
        for (int x = 0; x < n; x++) {
            bool iso = true;
            for (auto [u, v] : E) if (u == x || v == x) iso = false;
            bool cut = !iso && components(n, E, x) > base;
            assert(g.is_cut(x) == cut);
        }
    }
}

// C3C4：和暴力數三角形、四元環比
void test_c3c4() {
    for (int it = 0; it < 2000; it++) {
        int n = rnd(1, 8);
        vector<vector<int>> A(n, vector<int>(n));
        vector<int> deg(n);
        vector<array<int, 2>> E;
        for (int u = 0; u < n; u++) for (int v = u + 1; v < n; v++)
            if (rnd(0, 1)) A[u][v] = A[v][u] = 1, deg[u]++, deg[v]++, E.push_back({u, v});
        int c3 = 0, c4 = 0;
        for (int a = 0; a < n; a++) for (int b = a + 1; b < n; b++) for (int c = b + 1; c < n; c++) {
            c3 += A[a][b] && A[b][c] && A[a][c];
            for (int d = c + 1; d < n; d++)
                c4 += (A[a][b] && A[b][c] && A[c][d] && A[d][a]) + (A[a][b] && A[b][d] && A[d][c] && A[c][a])
                    + (A[a][c] && A[c][b] && A[b][d] && A[d][a]);
        }
        auto [r3, r4] = C3C4(deg, E);
        assert(r3 == c3 && r4 == c4);
    }
}

// 最大權閉合圖：值與回傳的點集都要對
void test_maximum_closure() {
    for (int it = 0; it < 2000; it++) {
        int n = rnd(1, 8);
        vector<int> w(n);
        for (auto &x : w) x = rnd(-6, 6);
        vector<pair<int, int>> E;
        for (int u = 0; u < n; u++) for (int v = 0; v < n; v++)
            if (u != v && rnd(0, 5) == 0) E.push_back({u, v});
        auto closed = [&](int s) {
            for (auto [u, v] : E) if ((s >> u & 1) && !(s >> v & 1)) return false;
            return true;
        };
        int best = 0;
        for (int s = 0; s < (1 << n); s++) if (closed(s)) {
            int sum = 0;
            for (int i = 0; i < n; i++) if (s >> i & 1) sum += w[i];
            best = max(best, sum);
        }
        auto [val, pick] = maximum_closure(w, E);
        assert(val == best);
        int s = 0, sum = 0;
        for (int i : pick) s |= 1 << i, sum += w[i];
        assert(closed(s) && sum == val);
    }
}

void test_mcmf() { // 改名後能編譯、能跑
    MCMF f(4);
    f.add(0, 1, 2, 1), f.add(0, 2, 1, 5), f.add(1, 3, 1, 1), f.add(2, 3, 2, 1), f.add(1, 2, 1, 1);
    auto [fl, cost] = f.flow(0, 3);
    assert(fl == 3 && cost == 2 + 3 + 6);
}

signed main() {
    test_two_sat();
    test_augment_path();
    test_is_cut();
    test_c3c4();
    test_maximum_closure();
    test_mcmf();
    cout << "OK\n";
}
