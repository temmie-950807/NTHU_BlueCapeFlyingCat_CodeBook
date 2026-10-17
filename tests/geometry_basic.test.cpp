// competitive-verifier: STANDALONE
// Geometry 01, 04–14：point 運算 / ori / btw / banana / rayHitSeg / abs2 / rotate / Arg、
// line build / ori / parallel / line_intersection、make_convex_hull、in_polygon 對暴力（__int128 精確）
// 小座標大量共線、重複、退化；大座標 ±1e9 檢查 #define int long long 下不溢位；T = int 與 T = ld 都測
#include "common.hpp"
#define pb push_back
mt19937 rngf(7122);
#include "geometry_lib.hpp"

using P = point<int>;
using PD = point<ld>;
using i128 = __int128;
mt19937_64 rng(7122);
int R(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }

i128 cr(P a, P b) { return (i128)a.x * b.y - (i128)a.y * b.x; }
i128 dt(P a, P b) { return (i128)a.x * b.x + (i128)a.y * b.y; }
P sub(P a, P b) { return P(a.x - b.x, a.y - b.y); }
int sg(i128 x) { return (x > 0) - (x < 0); }
int bori(P a, P b, P c) { return sg(cr(sub(b, a), sub(c, a))); }
bool eq(P a, P b) { return a.x == b.x && a.y == b.y; }
// c 在線段 ab 上（含端點），用 bounding box
bool bon(P a, P b, P c) {
    return bori(a, b, c) == 0 && min(a.x, b.x) <= c.x && c.x <= max(a.x, b.x) &&
           min(a.y, b.y) <= c.y && c.y <= max(a.y, b.y);
}
// num/den ∈ [0, 1]
bool in01(i128 num, i128 den) { return den > 0 ? (0 <= num && num <= den) : (den <= num && num <= 0); }
bool bseg(P a, P b, P c, P d) {
    if (eq(a, b)) return bon(c, d, a);
    if (eq(c, d)) return bon(a, b, c);
    P u = sub(b, a), v = sub(d, c), s = sub(c, a);
    i128 den = cr(u, v);
    if (den != 0) return in01(cr(s, v), den) && in01(cr(s, u), den);
    if (cr(s, u) != 0) return false;
    i128 t1 = dt(sub(c, a), u), t2 = dt(sub(d, a), u), L = dt(u, u);
    return max(t1, t2) >= 0 && min(t1, t2) <= L;
}
bool bray(P a, P b, P c, P d) {
    if (eq(a, b)) return bon(c, d, a);
    P u = sub(b, a), v = sub(d, c), s = sub(c, a);
    i128 den = cr(u, v);
    if (den != 0) {
        i128 t = cr(s, v);
        return (den > 0 ? t >= 0 : t <= 0) && in01(cr(s, u), den);
    }
    if (cr(s, u) != 0) return false;
    return max(dt(sub(c, a), u), dt(sub(d, a), u)) >= 0;
}
PD D(P p) { return PD(p.x, p.y); }

P smallP(int r) { return P(R(-r, r), R(-r, r)); }
P bigP() {
    static const int vals[] = {-1000000000, -999999999, 0, 1, 999999999, 1000000000};
    if (R(0, 1)) return P(vals[R(0, 5)], vals[R(0, 5)]);
    return P(R(-1e9, 1e9), R(-1e9, 1e9));
}
// 四個點：一半機率放在同一條線上
array<P, 4> quad(bool big) {
    array<P, 4> q;
    if (R(0, 1)) {
        P base = big ? P(R(-5e8, 5e8), R(-5e8, 5e8)) : smallP(2);
        int lim = big ? 100000000 : 1;
        P dir(R(-lim, lim), R(-lim, lim));
        for (auto &p : q) { int t = R(-4, 4); p = P(base.x + t * dir.x, base.y + t * dir.y); }
    } else
        for (auto &p : q) p = big ? bigP() : smallP(2);
    if (!R(0, 4)) q[R(0, 3)] = q[R(0, 3)];
    return q;
}

void test_point_ops() {
    for (int it = 0; it < 200000; it++) {
        bool big = it & 1;
        auto [a, b, c, d] = quad(big);
        assert(ori(a, b, c) == bori(a, b, c));
        assert(btw(a, b, c) == bon(a, b, c));
        assert(banana(a, b, c, d) == bseg(a, b, c, d));
        assert(rayHitSeg(a, b, c, d) == bray(a, b, c, d));
        assert((a ^ b) == cr(a, b) && (a * b) == dt(a, b) && abs2(a - b) == dt(sub(a, b), sub(a, b)));
        assert(eq(a + b, P(a.x + b.x, a.y + b.y)) && eq(a - b, sub(a, b)) && (a == b) == eq(a, b));
        if (!big) {
            assert(eq(a * 3, P(3 * a.x, 3 * a.y)) && eq((a * 6) / 3, a * 2));
            P r = a.rotate(b); // |r| = |a||b|，角度相加
            assert(dt(r, r) == dt(a, a) * dt(b, b));
            if (dt(a, a) && dt(b, b)) {
                ld ang = Arg(a) + Arg(b), ar = Arg(r);
                assert(fabs(remainder(ang - ar, 2 * M_PI)) < 1e-9);
            }
            assert(Arg(a) == (dt(a, a) ? atan2((ld)a.y, (ld)a.x) : 0));
            // ld 版本結果相同
            PD A = D(a), B = D(b), C = D(c), Dd = D(d);
            assert(ori(A, B, C) == bori(a, b, c) && btw(A, B, C) == bon(a, b, c));
            assert(banana(A, B, C, Dd) == bseg(a, b, c, d));
            assert(rayHitSeg(A, B, C, Dd) == bray(a, b, c, d));
            PD cv = (PD)a; assert(cv.x == a.x && cv.y == a.y);
        }
    }
}

void test_line() {
    for (int it = 0; it < 100000; it++) {
        bool big = it & 1;
        auto [a, b, c, d] = quad(big);
        if (eq(a, b) || eq(c, d)) continue;
        line<int> l(a, b), m(c, d);
        for (P p : {a, b}) assert((i128)l.a * p.x + (i128)l.b * p.y + l.c == 0);
        assert(sg((i128)l.a * c.x + (i128)l.b * c.y + l.c) == bori(a, b, c));
        assert(l.ori(c) == bori(a, b, c));
        assert(l.parallel(m) == (cr(sub(b, a), sub(d, c)) == 0));
        if (!l.parallel(m)) {
            P u = sub(b, a), v = sub(d, c), s = sub(c, a);
            long double t = (long double)cr(s, v) / (long double)cr(u, v);
            long double ex = a.x + u.x * t, ey = a.y + u.y * t;
            PD q = l.line_intersection(m);
            long double sc = max({1.0L, fabsl(ex), fabsl(ey), fabsl((long double)a.x), fabsl((long double)a.y), fabsl(u.x * t), fabsl(u.y * t)});
            assert(fabsl(q.x - ex) <= 1e-12 * sc && fabsl(q.y - ey) <= 1e-12 * sc);
            line<ld> L(D(a), D(b)), M(D(c), D(d));
            if (!big) {
                PD q2 = L.line_intersection(M);
                assert(fabsl(q2.x - ex) <= 1e-12 * sc && fabsl(q2.y - ey) <= 1e-12 * sc);
                PD cd = D(c);
                assert(L.ori(cd) == bori(a, b, c));
            }
        }
    }
}

// 暴力凸包：極點（simple=1）或邊界上所有點（simple=0），逆時針
vector<P> brute_hull(vector<P> v, bool simple) {
    sort(v.begin(), v.end(), [](P a, P b) { return make_pair(a.x, a.y) < make_pair(b.x, b.y); });
    v.erase(unique(v.begin(), v.end(), eq), v.end());
    int n = v.size();
    vector<P> ext, res;
    for (int i = 0; i < n; i++) { // v[i] 是極點 ⇔ 不在其他點的凸包內 ⇔ 存在…，這裡用：不在任一三角形/線段上
        bool in = false;
        for (int j = 0; j < n && !in; j++)
            for (int k = 0; k < n && !in; k++) {
                if (j == i || k == i) continue;
                if (bon(v[j], v[k], v[i])) in = true;
                for (int l = 0; l < n && !in; l++) {
                    if (l == i || l == j || l == k) continue;
                    int o1 = bori(v[j], v[k], v[i]), o2 = bori(v[k], v[l], v[i]), o3 = bori(v[l], v[j], v[i]);
                    if (bori(v[j], v[k], v[l]) && ((o1 >= 0 && o2 >= 0 && o3 >= 0) || (o1 <= 0 && o2 <= 0 && o3 <= 0))) in = true;
                }
            }
        if (!in) ext.push_back(v[i]);
    }
    if (ext.size() <= 2) return ext;
    for (P p : v) {
        bool keep = false;
        for (P e : ext) keep |= eq(p, e);
        int m = ext.size();
        if (!simple) // 在凸包某條邊上：該邊所在直線的同側包含所有點
            for (int i = 0; i < m; i++)
                for (int j = 0; j < m; j++) {
                    if (i == j || !bon(ext[i], ext[j], p)) continue;
                    bool ok = true;
                    for (P q : ext) ok &= bori(ext[i], ext[j], q) >= 0;
                    keep |= ok;
                }
        if (keep) res.push_back(p);
    }
    i128 cx = 0, cy = 0, k = ext.size();
    for (P e : ext) cx += e.x, cy += e.y;
    auto key = [&](P p) { return pair<i128, i128>(p.x * k - cx, p.y * k - cy); };
    auto half = [](pair<i128, i128> a) { return a.second < 0 || (a.second == 0 && a.first < 0); };
    sort(res.begin(), res.end(), [&](P a, P b) {
        auto A = key(a), B = key(b);
        if (half(A) != half(B)) return half(A) < half(B);
        return A.first * B.second - A.second * B.first > 0;
    });
    return res;
}
bool same_cyclic(const vector<P> &a, const vector<P> &b) {
    if (a.size() != b.size()) return false;
    if (a.empty()) return true;
    int n = a.size();
    for (int s = 0; s < n; s++) {
        bool ok = true;
        for (int i = 0; i < n && ok; i++) ok = eq(a[(s + i) % n], b[i]);
        if (ok) return true;
    }
    return false;
}
bool same_set(vector<P> a, vector<P> b) {
    auto c = [](P x, P y) { return make_pair(x.x, x.y) < make_pair(y.x, y.y); };
    sort(a.begin(), a.end(), c), sort(b.begin(), b.end(), c);
    return a.size() == b.size() && equal(a.begin(), a.end(), b.begin(), eq);
}

void test_hull() {
    for (int it = 0; it < 6000; it++) {
        int n = R(0, 9), r = R(1, 3);
        vector<P> v(n);
        if (R(0, 3) == 0) { // 全共線
            P base = smallP(2), dir = smallP(1);
            for (auto &p : v) { int t = R(-2, 2); p = P(base.x + t * dir.x, base.y + t * dir.y); }
        } else
            for (auto &p : v) p = smallP(r);
        auto exp1 = brute_hull(v, 1);
        polygon<int> h1(v); h1.make_convex_hull(1);
        if (exp1.size() <= 2) assert(same_set(h1.v, exp1));
        else assert(same_cyclic(h1.v, exp1));
        polygon<int> h0(v); h0.make_convex_hull(5); // simple 非 0/1 也要當 true
        assert(same_set(h0.v, h1.v));
        if (exp1.size() >= 3) {
            polygon<int> g(v); g.make_convex_hull(0);
            assert(same_cyclic(g.v, brute_hull(v, 0)));
        }
        vector<PD> vd;
        for (P p : v) vd.push_back(D(p));
        polygon<ld> hd(vd); hd.make_convex_hull(1);
        assert(hd.v.size() == h1.v.size());
        for (int i = 0; i < (int)hd.v.size(); i++) assert(hd.v[i].x == h1.v[i].x && hd.v[i].y == h1.v[i].y);
    }
    // 大座標：邊界上共線點（±1e9 角落），只確認 ori 不溢位、結果正確
    for (int it = 0; it < 300; it++) {
        vector<P> v;
        for (int i = 0; i < 8; i++) v.push_back(bigP());
        auto exp1 = brute_hull(v, 1);
        polygon<int> h(v); h.make_convex_hull(1);
        if (exp1.size() <= 2) assert(same_set(h.v, exp1));
        else assert(same_cyclic(h.v, exp1));
    }
}

int brute_in(const vector<P> &v, P a) {
    int n = v.size(), cnt = 0;
    for (int i = 0; i < n; i++) {
        P p = v[i], q = v[(i + 1) % n];
        if (bon(p, q, a)) return 0;
        if ((p.y > a.y) != (q.y > a.y)) { // 半開規則，交點在 a 右邊
            if (p.y > q.y) swap(p, q);
            if (bori(p, q, a) > 0) cnt++;
        }
    }
    return cnt & 1 ? 1 : -1;
}
// 星形簡單多邊形（對原點），可能有連續共線頂點、重複頂點
vector<P> star(int r) {
    while (true) {
        int n = R(3, 10);
        vector<P> v;
        for (int i = 0; i < n; i++) {
            P p = smallP(r);
            if (eq(p, P(0, 0))) continue;
            bool dup = false;
            for (P q : v) dup |= cr(p, q) == 0 && dt(p, q) > 0;
            if (!dup) v.push_back(p);
        }
        if (v.size() < 3) continue;
        auto half = [](P a) { return a.y < 0 || (a.y == 0 && a.x < 0); };
        sort(v.begin(), v.end(), [&](P a, P b) { return half(a) != half(b) ? half(a) < half(b) : cr(a, b) > 0; });
        bool ok = true;
        for (int i = 0; i < (int)v.size(); i++) ok &= cr(v[i], v[(i + 1) % v.size()]) > 0;
        if (!ok) continue;
        if (!R(0, 3)) { int i = R(0, v.size() - 1); v.insert(v.begin() + i, v[i]); }
        if (R(0, 1)) reverse(v.begin(), v.end()); // 順時針也要能用
        return v;
    }
}

void test_in_polygon() {
    for (int it = 0; it < 20000; it++) {
        int r = R(2, 4);
        auto v = star(r);
        polygon<int> poly(v);
        for (int x = -r - 1; x <= r + 1; x++)
            for (int y = -r - 1; y <= r + 1; y++) assert(poly.in_polygon(P(x, y)) == brute_in(v, P(x, y)));
    }
    // 座標放大到 ±1e9
    for (int it = 0; it < 3000; it++) {
        int K = 250000000;
        auto v = star(4);
        for (auto &p : v) p = P(p.x * K, p.y * K);
        polygon<int> poly(v);
        for (int q = 0; q < 50; q++) {
            P a;
            int t = R(0, 3);
            if (t == 0) a = v[R(0, v.size() - 1)];
            else if (t == 1) { P p = v[R(0, v.size() - 1)], s = v[R(0, v.size() - 1)]; a = P((p.x + s.x) / 2, (p.y + s.y) / 2); }
            else if (t == 2) a = P(R(-4, 4) * K + R(-1, 1), R(-4, 4) * K + R(-1, 1));
            else a = P(R(-1e9, 1e9), R(-1e9, 1e9));
            assert(poly.in_polygon(a) == brute_in(v, a));
        }
    }
}

signed main() {
    test_point_ops();
    test_line();
    test_hull();
    test_in_polygon();
    puts("OK");
    return 0;
}
