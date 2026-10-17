// competitive-verifier: STANDALONE
// halfplane_intersection（有界：加外框；含同向／反向平行線）交集非空時對裁切外框暴力（空交集是已知問題，不檢查）、
// minkowski（兩凸包各至少 2 點）對所有點對和的凸包
#include "common.hpp"
#define pb push_back
mt19937 rngf(7122);
#include "geometry_lib.hpp"

mt19937_64 rng(7122);
int R(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }

using PD = point<ld>;
ld area(vector<PD> v) {
    ld s = 0;
    for (int i = 0, n = v.size(); i < n; i++) s += v[i] ^ v[(i + 1) % n];
    return s / 2;
}
vector<PD> clip(vector<PD> poly, line<ld> L) { // 保留 L 左側（含線上）
    vector<PD> out;
    PD d = L.p2 - L.p1;
    auto val = [&](PD p) { return d ^ (p - L.p1); };
    for (int i = 0, n = poly.size(); i < n; i++) {
        PD a = poly[i], b = poly[(i + 1) % n];
        ld va = val(a), vb = val(b);
        if (va >= 0) out.push_back(a);
        if ((va > 0 && vb < 0) || (va < 0 && vb > 0)) out.push_back(a + (b - a) * (va / (va - vb)));
    }
    return out;
}

void test_hpi() {
    for (int it = 0; it < 20000; it++) {
        int B = R(3, 10), k = R(1, 12);
        vector<line<ld>> s = {line<ld>(PD(-B, -B), PD(B, -B)), line<ld>(PD(B, -B), PD(B, B)),
                              line<ld>(PD(B, B), PD(-B, B)), line<ld>(PD(-B, B), PD(-B, -B))};
        for (int i = 0; i < k; i++) {
            point<int> d(R(-4, 4), R(-4, 4));
            if (d == point<int>(0, 0)) { i--; continue; }
            PD p(R(-B, B), R(-B, B));
            s.push_back(line<ld>(p, p + PD(d.x, d.y)));
        }
        vector<PD> box = {PD(-B, -B), PD(B, -B), PD(B, B), PD(-B, B)};
        for (auto &L : s) box = clip(box, L);
        ld ea = box.size() >= 3 ? area(box) : 0;
        shuffle(s.begin(), s.end(), rng);
        polygon<ld> P;
        int cnt = halfplane_intersection(s, P);
        assert(cnt == (int)P.v.size());
        if (ea < 1e-9) continue; // 空交集：已知會回傳負面積
        ld ga = area(P.v);
        if (fabs(ga - ea) > 1e-6) {
            printf("area mismatch exp=%.9f got=%.9f\n", (double)ea, (double)ga);
            for (auto &L : s) printf("  (%g,%g)->(%g,%g)\n", L.p1.x, L.p1.y, L.p2.x, L.p2.y);
            exit(1);
        }
        for (auto &p : P.v) for (auto &L : s) assert(((L.p2 - L.p1) ^ (p - L.p1)) > -1e-6);
    }
}

polygon<int> rand_hull(int c, int lo) {
    while (true) {
        polygon<int> g;
        int k = R(lo, 8);
        for (int i = 0; i < k; i++) g.v.push_back(point<int>(R(-c, c), R(-c, c)));
        g.make_convex_hull(1);
        if (g.v.size() >= 2) return g;
    }
}
void test_minkowski() {
    for (int it = 0; it < 20000; it++) {
        auto a = rand_hull(R(1, 5), 2), b = rand_hull(R(1, 5), 2);
        polygon<int> h, m;
        for (auto &p : a.v) for (auto &q : b.v) h.v.push_back(p + q);
        h.make_convex_hull(1);
        m.minkowski(a.v, b.v);
        auto norm = [](vector<point<int>> v) {
            auto it = min_element(v.begin(), v.end(), [](auto &p, auto &q) { return tie(p.y, p.x) < tie(q.y, q.x); });
            rotate(v.begin(), it, v.end());
            return v;
        };
        auto x = norm(h.v), y = norm(m.v);
        bool same = x.size() == y.size();
        for (int i = 0; same && i < (int)x.size(); i++) same = x[i] == y[i];
        if (!same) {
            printf("minkowski mismatch\n a:"); for (auto &p : a.v) cout << p; printf("\n b:"); for (auto &p : b.v) cout << p;
            printf("\n exp:"); for (auto &p : x) cout << p; printf("\n got:"); for (auto &p : y) cout << p; puts("");
            exit(1);
        }
    }
}

signed main() {
    test_hpi();
    test_minkowski();
    puts("ok");
    return 0;
}
