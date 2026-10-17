// competitive-verifier: STANDALONE
// 凸包查詢 15~20（in_convex、cycle_search、line_cut_convex、convex_line_intersect、
// segment_pass_convex_interior（不含共線）、convex_tangent_point（點在外面／頂點上／邊上））對暴力
#include "common.hpp"
#define pb push_back
mt19937 rngf(7122);
#include "geometry_lib.hpp"

using P = point<int>;
mt19937_64 rng(7122);
int R(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }
P rp(int c) { return P(R(-c, c), R(-c, c)); }

polygon<int> rand_hull(int c) { // 任三點不共線、逆時針、至少 3 點
    while (true) {
        polygon<int> g;
        int k = R(3, 12);
        for (int i = 0; i < k; i++) g.v.push_back(rp(c));
        g.make_convex_hull(1);
        if (g.v.size() >= 3) return g;
    }
}
P edge_pt(polygon<int> &g, P p) { // 隨機一條邊上的格點（沒有就回傳 p）
    int n = g.v.size(), i = R(0, n - 1);
    P e = g.v[(i + 1) % n] - g.v[i];
    int k = __gcd(llabs(e.x), llabs(e.y));
    return k > 1 ? g.v[i] + e / k * R(1, k - 1) : p;
}
int brute_in(polygon<int> &g, P p) {
    int n = g.v.size(), in = 1;
    for (int i = 0; i < n; i++) {
        P a = g.v[i], b = g.v[(i + 1) % n];
        if (btw(a, b, p)) return 0;
        if (ori(a, b, p) <= 0) in = 0;
    }
    return in ? 1 : -1;
}
// 線段 p(t) = a + t(b-a), t∈[0,1] 落在閉凸包裡的區間 [lo, hi]，回傳 {1:lo<hi, 0:lo==hi, -1:空}
int brute_seg(polygon<int> &g, P a, P b) {
    int n = g.v.size();
    // 分數 num/den, den > 0
    __int128 ln = 0, ld_ = 1, hn = 1, hd = 1;
    for (int i = 0; i < n; i++) {
        P s = g.v[i], e = g.v[(i + 1) % n] - s;
        int A = e ^ (a - s), B = e ^ (b - a); // A + B t >= 0
        if (B == 0) { if (A < 0) return -1; continue; }
        __int128 num = -A, den = B;
        if (den < 0) num = -num, den = -den;
        if (B > 0) { if (num * ld_ > ln * den) ln = num, ld_ = den; } // t >= -A/B
        else { if (num * hd < hn * den) hn = num, hd = den; }         // t <= -A/B
    }
    __int128 l = ln * hd, h = hn * ld_;
    return l < h ? 1 : (l == h ? 0 : -1);
}

signed main() {
    for (int it = 0; it < 30000; it++) {
        int c = R(1, 3) == 1 ? 3 : 8;
        auto g = rand_hull(c);
        int n = g.v.size();
        // in_convex
        for (int t = 0; t < 5; t++) {
            P p = t == 0 ? g.v[R(0, n - 1)] : t == 1 ? edge_pt(g, rp(c + 1)) : rp(c + 1);
            assert(g.in_convex(p) == brute_in(g, p));
        }
        // cycle_search：f(x, y) = v[x]·d > v[y]·d，回傳最大值之一
        for (int t = 0; t < 3; t++) {
            P d = t == 0 ? (g.v[(R(0, n - 1))] - g.v[0]).rotate(P(0, 1)) : rp(5);
            if (d == P(0, 0)) continue;
            auto f = [&](int x, int y) { return g.v[x] * d > g.v[y] * d; };
            int k = g.cycle_search(f), mx = LLONG_MIN;
            for (auto &q : g.v) mx = max(mx, q * d);
            assert(g.v[k] * d == mx);
        }
        // line_cut_convex、convex_line_intersect
        for (int t = 0; t < 4; t++) {
            P a = t == 0 ? g.v[R(0, n - 1)] : rp(c + 1), b = t <= 1 ? g.v[R(0, n - 1)] : rp(c + 1);
            if (a == b) continue;
            line<int> L(a, b);
            int mn = LLONG_MAX, mx = LLONG_MIN;
            vector<int> s(n);
            for (int i = 0; i < n; i++) s[i] = (b - a) ^ (g.v[i] - a), mn = min(mn, s[i]), mx = max(mx, s[i]);
            int exp = (mn > 0 || mx < 0) ? -1 : (mn == 0 || mx == 0) ? 0 : 1;
            assert(g.line_cut_convex(L) == exp);
            vector<pair<int, int>> e;
            for (int i = 0; i < n; i++) {
                if (s[i] == 0) e.push_back({i, i});
                else if (s[i] * s[(i + 1) % n] < 0) e.push_back({i, (i + 1) % n});
            }
            auto got = g.convex_line_intersect(L);
            sort(e.begin(), e.end()), sort(got.begin(), got.end());
            assert(got == e);
        }
        // segment_pass_convex_interior（排除線段所在直線包含某條邊：已知共線問題）
        for (int t = 0; t < 4; t++) {
            P a = t == 0 ? g.v[R(0, n - 1)] : rp(c + 1), b = t == 1 ? g.v[R(0, n - 1)] : rp(c + 1);
            if (a == b) continue;
            bool col = false;
            for (int i = 0; i < n; i++)
                if (((b - a) ^ (g.v[i] - a)) == 0 && ((b - a) ^ (g.v[(i + 1) % n] - a)) == 0) col = true;
            if (col) continue;
            assert(g.segment_pass_convex_interior(line<int>(a, b)) == brute_seg(g, a, b));
        }
        // convex_tangent_point
        for (int t = 0; t < 4; t++) {
            P p = t == 0 ? g.v[R(0, n - 1)] : rp(c + 2);
            if (t == 1) p = edge_pt(g, p);
            int bi = brute_in(g, p);
            if (bi == 1) continue;
            auto [x, y] = g.convex_tangent_point(p);
            assert(0 <= x && x < n && 0 <= y && y < n);
            int z = -1;
            for (int i = 0; i < n; i++) if (g.v[i] == p) z = i;
            if (z != -1) { assert(x == (z + n - 1) % n && y == (z + 1) % n); continue; }
            int edg = -1;
            for (int i = 0; i < n; i++) if (btw(g.v[i], g.v[(i + 1) % n], p)) edg = i;
            if (edg != -1) { assert(x == edg && y == (edg + 1) % n); continue; }
            auto side = [&](int k) { // 1：所有點都在 p->v[k] 左邊（含線上）、-1：右邊
                bool L = true, Rr = true;
                for (auto &q : g.v) { int o = ori(p, g.v[k], q); if (o < 0) L = false; if (o > 0) Rr = false; }
                return L ? 1 : Rr ? -1 : 0;
            };
            int sx = side(x), sy = side(y);
            assert(sx != 0 && sy != 0 && sx != sy);
        }
    }
    puts("ok");
    return 0;
}
