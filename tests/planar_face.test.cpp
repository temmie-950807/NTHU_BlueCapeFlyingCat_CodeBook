// competitive-verifier: STANDALONE
#include "common.hpp"
#define pb push_back // geometry_lib 已知未修的問題，見 tests/build_geometry.py
mt19937 rngf(7122);
#include "geometry_lib.hpp"
#include "../codes/Graph/EnumeratePlanarFace.cpp"

mt19937 rng(7122);
int rnd(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }
using P = point<int>;

int area2(vector<P> v, const vector<int> &idx) { // 有號兩倍面積（逆時針為正）
    int s = 0;
    for (int i = 0; i < (int)idx.size(); i++) s += v[idx[i]] ^ v[idx[(i + 1) % idx.size()]];
    return s;
}

// 跑一次並檢查共通性質，回傳排序後的面積
vector<int> run(vector<P> v, vector<pair<int, int>> E, int comps) {
    int n = v.size(), m = E.size();
    PlanarGraph g(n, v);
    for (auto [a, b] : E) g.add_edge(a, b);
    auto ret = g.enumerate_face();
    assert((int)ret.size() == g.faces && (int)g.F.size() == m + 1 + g.faces && (int)g.D.size() == m + 1 + g.faces);
    int iso = 0;
    for (int i = 0; i < n; i++) iso += g.G[i].empty();
    assert(g.faces == m - (n - iso) + (comps - iso)); // Euler：每個連通塊各有一個外面
    for (int k = 0; k < g.faces; k++) assert(ret[k] > 0 && ret[k] == -area2(v, g.F[m + 1 + k]));
    vector<int> cnt(m); // 每條邊的兩個 half-edge 各屬於一個面
    for (int f = m; f <= m + g.faces; f++) for (auto [u, w] : g.D[f]) assert(w == 0), cnt[u]++;
    for (int u = 0; u < m; u++) assert(cnt[u] == 2);
    sort(ret.begin(), ret.end());
    return ret;
}

signed main() {
    // 凸多邊形 + 扇形三角剖分：每個面是三角形 (0, i, i + 1)
    for (int it = 0; it < 300; it++) {
        int k = rnd(3, 12);
        vector<P> v;
        for (int i = 0; i < k; i++) { // 正多邊形放大取整後仍是凸的
            double t = 2 * acos(-1) * i / k;
            v.push_back({(int)llround(1000 * cos(t)), (int)llround(1000 * sin(t))});
        }
        vector<pair<int, int>> E;
        for (int i = 0; i < k; i++) E.push_back(it % 2 ? pair{i, (i + 1) % k} : pair{(i + 1) % k, i});
        for (int i = 2; i + 1 < k; i++) E.push_back({0, i});
        shuffle(E.begin(), E.end(), rng);
        vector<int> want;
        for (int i = 1; i + 1 < k; i++) want.push_back(area2(v, {0, i, i + 1}));
        sort(want.begin(), want.end());
        assert(run(v, E, 1) == want);
    }
    // 格子圖：每格都是面積 2 的面
    for (int r = 1; r <= 6; r++) for (int c = 1; c <= 6; c++) {
        vector<P> v;
        vector<pair<int, int>> E;
        for (int i = 0; i <= r; i++) for (int j = 0; j <= c; j++) v.push_back({j, i});
        auto id = [&](int i, int j) { return i * (c + 1) + j; };
        for (int i = 0; i <= r; i++) for (int j = 0; j <= c; j++) {
            if (j < c) E.push_back({id(i, j), id(i, j + 1)});
            if (i < r) E.push_back({id(i + 1, j), id(i, j)});
        }
        assert(run(v, E, 1) == vector<int>(r * c, 2));
    }
    { // 兩個分開的正方形 + 一條懸掛邊 + 一棵樹 + 孤點
        vector<P> v = {{0, 0}, {2, 0}, {2, 2}, {0, 2}, {1, 1}, {10, 0}, {13, 0}, {13, 3}, {10, 3}, {20, 20}, {21, 20}, {20, 25}, {30, 30}};
        vector<pair<int, int>> E = {{0, 1}, {1, 2}, {2, 3}, {3, 0}, {0, 4}, {5, 6}, {6, 7}, {7, 8}, {8, 5}, {9, 10}, {9, 11}};
        assert(run(v, E, 4) == vector<int>({8, 18}));
    }
    { // 大正方形裡面有一個三角形（洞）：大正方形的面積不會扣掉洞
        vector<P> v = {{0, 0}, {10, 0}, {10, 10}, {0, 10}, {2, 2}, {5, 2}, {2, 5}};
        vector<pair<int, int>> E = {{0, 1}, {1, 2}, {2, 3}, {3, 0}, {4, 5}, {5, 6}, {6, 4}};
        assert(run(v, E, 2) == vector<int>({9, 200}));
    }
    return 0;
}
