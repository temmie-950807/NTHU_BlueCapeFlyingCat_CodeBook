// competitive-verifier: STANDALONE
// Cir：covers、Cir_intersect（不相切、不同心）、point_tangent（點在圓外）、circle_tangent（內外公切線、等半徑、內含、相切）
// 對解析檢查；minimum_enclosing_circle（相異點、含共線）對 O(n^4) 暴力，另測 n = 1e5 的時間
#include "common.hpp"
#define pb push_back
mt19937 rngf(7122);
#include "geometry_lib.hpp"

mt19937_64 rng(7122);
int R(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }
using PD = point<ld>;
const ld E = 1e-6;
ld dist(PD a, PD b) { return sqrt(abs2(a - b)); }
ld line_dist(PD o, PD a, PD b) { return fabs((b - a) ^ (o - a)) / dist(a, b); }

void test_basic() {
    for (int it = 0; it < 100000; it++) {
        Cir a{PD(R(-6, 6), R(-6, 6)), (ld)R(1, 6)}, b{PD(R(-6, 6), R(-6, 6)), (ld)R(1, 6)};
        int d2 = abs2(a.o - b.o), s = (a.r + b.r) * (a.r + b.r), t = (a.r - b.r) * (a.r - b.r);
        // covers：精確整數判斷 d + b.r <= a.r
        bool cov = b.r <= a.r && d2 <= t;
        assert(a.covers(b) == cov);
        // Cir_intersect
        if (d2 != 0 && d2 != s && d2 != t) {
            auto v = a.Cir_intersect(b);
            int exp = (t < d2 && d2 < s) ? 2 : 0;
            assert((int)v.size() == exp);
            for (auto &p : v) assert(fabs(dist(p, a.o) - a.r) < E && fabs(dist(p, b.o) - b.r) < E);
            if (exp == 2) assert(dist(v[0], v[1]) > E);
        }
        // point_tangent：點在圓外
        PD p(R(-8, 8), R(-8, 8));
        if (abs2(p - a.o) > a.r * a.r) {
            auto v = a.point_tangent(p);
            assert(v.size() == 2 && dist(v[0], v[1]) > E);
            for (auto &q : v) assert(fabs(dist(q, a.o) - a.r) < E && fabs((q - a.o) * (q - p)) < E);
        }
        // circle_tangent：允許其中一個半徑為 0
        if (R(0, 4) == 0) b.r = 0;
        d2 = abs2(a.o - b.o), s = (a.r + b.r) * (a.r + b.r), t = (a.r - b.r) * (a.r - b.r);
        for (int s1 : {1, -1}) {
            auto v = a.circle_tangent(b, s1);
            int lim = s1 == 1 ? t : s, exp = d2 == 0 ? 0 : d2 > lim ? 2 : d2 == lim ? 1 : 0;
            assert((int)v.size() == exp);
            for (auto &[p1, p2] : v) {
                // 兩圓相切於一點時 p2 會被換成切線上的另一點（p1 同時在兩圓上）
                bool touch = fabs(dist(p1, b.o) - b.r) < E;
                assert(fabs(dist(p1, a.o) - a.r) < E && (touch || fabs(dist(p2, b.o) - b.r) < E));
                assert(dist(p1, p2) > E);
                assert(fabs(line_dist(a.o, p1, p2) - a.r) < E && fabs(line_dist(b.o, p1, p2) - b.r) < E);
                ld sa = (p2 - p1) ^ (a.o - p1), sb = (p2 - p1) ^ (b.o - p1);
                if (a.r > 0 && b.r > 0) assert(s1 == 1 ? sa * sb > 0 : sa * sb < 0);
            }
            if (exp == 2) { // 兩條要不同
                auto [p, q] = v[0]; auto [x, y] = v[1];
                assert(line_dist(x, p, q) > E || line_dist(y, p, q) > E);
            }
        }
    }
}

ld brute_mec(vector<PD> &p) {
    int n = p.size();
    ld best = 1e18;
    auto ok = [&](PD c, ld r) { for (auto &q : p) if (dist(q, c) > r + 1e-7) return false; return true; };
    if (n == 1) return 0;
    for (int i = 0; i < n; i++) for (int j = i + 1; j < n; j++) {
        PD c = (p[i] + p[j]) / 2; ld r = dist(p[i], c);
        if (ok(c, r)) best = min(best, r);
        for (int k = j + 1; k < n; k++) {
            if (ori(p[i], p[j], p[k]) == 0) continue;
            PD b = p[j] - p[i], c = p[k] - p[i]; ld D = 2 * (b ^ c);
            PD cc = p[i] + PD(c.y * abs2(b) - b.y * abs2(c), b.x * abs2(c) - c.x * abs2(b)) / D; ld rr = dist(cc, p[i]);
            if (ok(cc, rr)) best = min(best, rr);
        }
    }
    return best;
}
void test_mec() {
    for (int it = 0; it < 20000; it++) {
        int c = R(1, 6), n = R(1, 9), col = R(0, 3) == 0;
        set<pair<int, int>> S;
        point<int> base(R(-c, c), R(-c, c)), dir(R(-2, 2), R(-2, 2));
        if (dir == point<int>(0, 0)) dir = point<int>(1, 0);
        while ((int)S.size() < n) {
            if (col) { auto q = base + dir * R(-4, 4); S.insert({q.x, q.y}); }
            else S.insert({R(-c, c), R(-c, c)});
            if (!col && (int)S.size() > (2 * c + 1) * (2 * c + 1) - 1) break;
        }
        vector<PD> p;
        for (auto [x, y] : S) p.push_back(PD(x, y));
        Cir C; C.min_enclosing(p);
        ld e = brute_mec(p);
        assert(fabs(C.r - e) < 1e-6);
        for (auto &q : p) assert(dist(q, C.o) < C.r + 1e-6);
    }
    // 時間：n = 1e5 隨機點、共圓點
    for (int t = 0; t < 2; t++) {
        vector<PD> p;
        for (int i = 0; i < 100000; i++) {
            if (t == 0) p.push_back(PD(R(-1e9, 1e9), R(-1e9, 1e9)));
            else { ld a = 2 * acos(-1.0) * i / 100000; p.push_back(PD(1e6 * cos(a), 1e6 * sin(a))); }
        }
        auto st = clock();
        Cir C; C.min_enclosing(p);
        double sec = double(clock() - st) / CLOCKS_PER_SEC;
        fprintf(stderr, "mec n=1e5 t=%lld: %.3fs r=%.6f\n", t, sec, (double)C.r);
        for (auto &q : p) assert(dist(q, C.o) < C.r * (1 + 1e-9) + 1e-6);
        assert(sec < 0.5);
    }
}

signed main() {
    test_basic();
    test_mec();
    puts("ok");
    return 0;
}
