// competitive-verifier: STANDALONE
#include "common.hpp"
int qp(int b, int p, int m) { // NTT 需要的快速冪
    int r = 1;
    for (b %= m; p; p >>= 1, b = b * b % m) if (p & 1) r = r * b % m;
    return r;
}
#include "../codes/Convolution/NTT_mod_998244353.cpp"
#include "../codes/Convolution/PolyInv.cpp"
#include "../codes/Convolution/PolySqrt.cpp"

// PolyInv / PolySqrt 與暴力 O(n^2) 比對
vector<int> mulTrunc(const vector<int> &a, const vector<int> &b, int n) {
    vector<int> c(n);
    for (int i = 0; i < n && i < (int)a.size(); i++)
        for (int j = 0; i + j < n && j < (int)b.size(); j++) c[i + j] = (c[i + j] + a[i] * b[j]) % MOD;
    return c;
}
signed main() {
    mt19937_64 rng(7122);
    vector<int> sizes{1, 2, 3, 4, 5, 7, 8, 9, 15, 16, 17, 31, 32, 33, 64, 100, 128, 129, 257};
    for (int it = 0; it < 200; it++) {
        int n = it < (int)sizes.size() ? sizes[it] : rng() % 200 + 1;
        vector<int> a(n);
        for (auto &x : a) x = it % 4 ? rng() % MOD : MOD - 1;
        if (a[0] == 0) a[0] = 1;
        // 反元素：a * inv = 1 (mod x^n)
        vector<int> inv = polyInv(a), one(n);
        one[0] = 1;
        assert((int)inv.size() == n && mulTrunc(a, inv, n) == one);
        // 平方根：先造 g^2，前面補 2s 個 0（偶數位移）
        int s = it % 3 ? 0 : rng() % (n + 1) / 2;
        vector<int> g(n), sq(n);
        for (auto &x : g) x = rng() % MOD;
        if (g[0] == 0) g[0] = 1;
        vector<int> g2 = mulTrunc(g, g, n);
        for (int i = 2 * s; i < n; i++) sq[i] = g2[i - 2 * s];
        vector<int> r = polySqrt(sq);
        assert((int)r.size() == n && mulTrunc(r, r, n) == sq);
        // 奇數位移 -> 無解
        if (n >= 2) {
            vector<int> odd(n);
            odd[1] = 5;
            for (int i = 2; i < n; i++) odd[i] = rng() % MOD;
            assert(polySqrt(odd).empty());
        }
        // 常數項非二次剩餘 (3 是 998244353 的原根) -> 無解
        vector<int> nr = a;
        nr[0] = 3;
        assert(polySqrt(nr).empty());
        // 全 0
        assert(polySqrt(vector<int>(n, 0)) == vector<int>(n, 0));
    }
    // 非零項在很後面：x^{n-1}（n-1 偶數）與 x^{n-1}（奇數）
    for (int n : {1, 2, 5, 6, 9}) {
        vector<int> a(n);
        a[n - 1] = 4;
        vector<int> r = polySqrt(a);
        if ((n - 1) % 2) assert(r.empty());
        else assert((int)r.size() == n && mulTrunc(r, r, n) == a);
    }
    for (int x = 0; x < 2000; x++) { // mod_sqrt
        int y = mod_sqrt(x), leg = qp(x, (MOD - 1) / 2, MOD);
        if (x && leg == MOD - 1) assert(y == -1);
        else assert(y * y % MOD == x);
    }
    // 大小 2^17 與自身相乘驗證
    int n = 1 << 17;
    vector<int> a(n);
    for (auto &x : a) x = rng() % MOD;
    a[0] = 1;
    vector<int> c = polyMul(a, polyInv(a));
    assert(c[0] == 1);
    for (int i = 1; i < n; i++) assert(c[i] == 0);
    vector<int> r = polySqrt(a), rr = polyMul(r, r);
    rr.resize(n);
    assert(rr == a);
    puts("OK");
    return 0;
}
