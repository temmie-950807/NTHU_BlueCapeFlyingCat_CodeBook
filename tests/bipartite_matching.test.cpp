// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/bipartitematching
#include <cassert>
#include "common.hpp"
#include "../codes/Graph/Augment_Path.cpp"

signed main() {
    ios::sync_with_stdio(0), cin.tie(0);
    int L, R, M;
    cin >> L >> R >> M;
    AugmentPath g(L, R);
    vector<pair<int, int>> E(M);
    for (auto &[a, b] : E) cin >> a >> b, g.add(a, b);
    auto mt = g.find_max_matching();
    // König：最小點覆蓋大小 = 最大匹配，且要蓋住每條邊（R > L 時會用到 visy 的全部長度）
    auto vc = g.find_min_vertex_cover();
    assert(vc.size() == mt.size());
    vector<int> cx(L), cy(R);
    for (auto [s, i] : vc) (s == 1 ? cx : cy)[i] = 1;
    for (auto [a, b] : E) assert(cx[a] || cy[b]);
    cout << mt.size() << '\n';
    for (auto [a, b] : mt) cout << a << ' ' << b << '\n';
}
