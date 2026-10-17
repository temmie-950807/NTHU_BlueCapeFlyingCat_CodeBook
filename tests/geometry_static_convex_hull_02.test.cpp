// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/static_convex_hull
// Geometry/02_convex_hull.cpp（獨立版）polygon::make_convex_hull(simple = 1)
#include "common.hpp"
#include "../codes/Geometry/02_convex_hull.cpp"

signed main() {
    ios::sync_with_stdio(false), cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        polygon<int> g;
        g.v.resize(n);
        for (auto &p : g.v) cin >> p.x >> p.y;
        g.make_convex_hull(1);
        cout << g.v.size() << '\n';
        for (auto &p : g.v) cout << p.x << ' ' << p.y << '\n';
    }
}
