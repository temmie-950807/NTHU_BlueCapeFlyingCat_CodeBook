// competitive-verifier: STANDALONE
// Geometry/02_convex_hull.cpp（獨立版凸包）對性質檢查：simple = 0/1、重複點、全共線、±1e9 大座標
#include "common.hpp"
#include "../codes/Geometry/02_convex_hull.cpp"

using P = point<int>;
mt19937_64 rng(7122);
int R(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }
int bori(P a, P b, P c) {
    __int128 v = (__int128)(b.x - a.x) * (c.y - a.y) - (__int128)(b.y - a.y) * (c.x - a.x);
    return (v > 0) - (v < 0);
}
// r 在線段 pq 上；strict 時不含端點
bool on(P p, P q, P r, bool strict) {
    if (bori(p, q, r)) return false;
    bool in = min(p.x, q.x) <= r.x && r.x <= max(p.x, q.x) && min(p.y, q.y) <= r.y && r.y <= max(p.y, q.y);
    return in && !(strict && (r == p || r == q));
}

void check(vector<P> pts, int simple) {
    polygon<int> g; g.v = pts; g.make_convex_hull(simple);
    auto &h = g.v;
    set<pair<int, int>> in, out;
    for (P p : pts) in.insert({p.x, p.y});
    for (P p : h) assert(in.count({p.x, p.y})), out.insert({p.x, p.y});
    if (in.size() <= 1) { assert(h.size() == in.size()); return; }
    bool col = true;
    for (P r : pts) col &= bori(pts[0], *find_if(pts.begin(), pts.end(), [&](P q) { return !(q == pts[0]); }), r) == 0;
    if (col) { // 全共線：simple = 1 回傳兩端點；simple = 0 目前會來回走一次（含重複點），不檢查
        if (!simple) return;
        assert(h.size() == 2 && !(h[0] == h[1]));
        for (P r : pts) assert(on(h[0], h[1], r, false));
        return;
    }
    int m = h.size();
    assert(m >= 3 && (int)out.size() == m); // 無重複
    for (int i = 0; i < m; i++) {
        P p = h[i], q = h[(i + 1) % m];
        for (P r : pts) {
            assert(bori(p, q, r) >= 0);
            if (simple) assert(bori(p, q, r) > 0 || on(p, q, r, false)); // pq 是極大的邊
            else assert(!on(p, q, r, true));                            // 邊上的點都要留下
        }
    }
}

signed main() {
    for (int it = 0; it < 30000; it++) {
        int n = R(0, 10), r = R(1, 3);
        vector<P> v(n);
        if (R(0, 3) == 0) {
            P b(R(-2, 2), R(-2, 2)), d(R(-1, 1), R(-1, 1));
            for (auto &p : v) { int t = R(-2, 2); p = P(b.x + t * d.x, b.y + t * d.y); }
        } else
            for (auto &p : v) p = P(R(-r, r), R(-r, r));
        check(v, 1), check(v, 0), check(v, 7);
    }
    const int vals[] = {-1000000000, -999999999, 0, 999999999, 1000000000};
    for (int it = 0; it < 3000; it++) {
        vector<P> v(R(1, 12));
        for (auto &p : v) p = R(0, 1) ? P(vals[R(0, 4)], vals[R(0, 4)]) : P(R(-1e9, 1e9), R(-1e9, 1e9));
        check(v, 1), check(v, 0);
    }
    puts("OK");
    return 0;
}
