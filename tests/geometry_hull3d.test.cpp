// competitive-verifier: STANDALONE
// Geometry_Struct_3D hull3（相異點、無四點共面）：面集合、朝外法向量、面數 2h-4 對 O(n^4) 暴力；T = long long 與 long double
#include "common.hpp"
#define all(x) (x).begin(), (x).end()
#define emplace_back(...) push_back({__VA_ARGS__}) // 已知：face 是 aggregate，C++17 不能 emplace_back
#include "../codes/Geometry/Geometry_Struct_3D.cpp"
#undef emplace_back

mt19937_64 rng(7122);
int R(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }

template <class T> void run(int iters) {
    using P = pt3<T>;
    for (int it = 0; it < iters; it++) {
        int n = R(4, 12), c = R(2, 6);
        vector<P> v;
        auto gp = [&](P q) { // 無三點共線、無四點共面
            for (auto &a : v) if (a == q) return false;
            for (int i = 0; i < (int)v.size(); i++) for (int j = i + 1; j < (int)v.size(); j++) {
                P cr = (v[j] - v[i]) ^ (q - v[i]);
                if (cr == P(0, 0, 0)) return false;
                for (int k = j + 1; k < (int)v.size(); k++) if (((v[j] - v[i]) ^ (v[k] - v[i])) * (q - v[i]) == 0) return false;
            }
            return true;
        };
        for (int tries = 0; (int)v.size() < n && tries < 1000; tries++) {
            P q(R(-c, c), R(-c, c), R(-c, c));
            if (gp(q)) v.push_back(q);
        }
        n = v.size();
        if (n < 4) continue;
        // 暴力：所有其他點都在同一側的三角形
        set<array<tuple<T, T, T>, 3>> exp;
        set<int> on;
        for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) for (int k = 0; k < n; k++) {
            if (i == j || j == k || i == k) continue;
            P q = (v[j] - v[i]) ^ (v[k] - v[i]);
            bool ok = true;
            for (int l = 0; l < n; l++) if ((v[l] - v[i]) * q > 0) ok = false;
            if (!ok || !(i < j && i < k)) continue; // 以最小 index 開頭、保持方向
            auto t = [&](int x) { return make_tuple(v[x].x, v[x].y, v[x].z); };
            exp.insert({t(i), t(j), t(k)});
            on.insert(i), on.insert(j), on.insert(k);
        }
        vector<P> w = v;
        shuffle(w.begin(), w.end(), rng);
        auto f = hull3(w);
        assert(f.size() == exp.size() && (int)f.size() == 2 * (int)on.size() - 4);
        set<array<tuple<T, T, T>, 3>> got;
        for (auto &F : f) {
            int a[3] = {F.a, F.b, F.c};
            rotate(a, min_element(a, a + 3), a + 3);
            auto t = [&](int x) { return make_tuple(w[x].x, w[x].y, w[x].z); };
            got.insert({t(a[0]), t(a[1]), t(a[2])});
            P q = (w[F.b] - w[F.a]) ^ (w[F.c] - w[F.a]);
            assert(q == F.q);
        }
        // 暴力以 v 的 index 最小者開頭；換成座標後用旋轉到字典序最小者比較
        auto canon = [](set<array<tuple<T, T, T>, 3>> s) {
            set<array<tuple<T, T, T>, 3>> r;
            for (auto x : s) { rotate(x.begin(), min_element(x.begin(), x.end()), x.end()); r.insert(x); }
            return r;
        };
        assert(canon(got) == canon(exp));
    }
}

signed main() {
    run<long long>(3000);
    run<ld>(3000);
    puts("ok");
    return 0;
}
