// competitive-verifier: STANDALONE
// Matrix det（負數元素）、Lagrange_continuous_x（x < X_0）、phi_1_to_n（n = 0）、FWT 對暴力
#include "common.hpp"
#include <cassert> // GCC 16 的 bits/stdc++.h 不含 assert
const int MOD = 998244353;
int qp(int b, int p, int m) {
    int r = 1;
    for (b %= m; p; p >>= 1, b = b * b % m) if (p & 1) r = r * b % m;
    return r;
}
#include "../codes/Math/Matrix.cpp"
#include "../codes/Math/Lagrange_continuous_x.cpp"
#include "../codes/Math/歐拉公式.cpp"
#include "../codes/Convolution/FWT.cpp"

mt19937_64 rng(7122);
int R(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }

int brute_det(vector<vector<int>> a) { // 精確整數，n 小
    int n = a.size(), s = 0;
    vector<int> p(n);
    iota(p.begin(), p.end(), 0);
    do {
        int t = 1, inv = 0;
        for (int i = 0; i < n; i++) {
            t *= a[i][p[i]];
            for (int j = i + 1; j < n; j++) inv += p[i] > p[j];
        }
        s += inv & 1 ? -t : t;
    } while (next_permutation(p.begin(), p.end()));
    return (s % MOD + MOD) % MOD;
}

signed main() {
    for (int it = 0; it < 2000; it++) {
        int n = R(1, 6);
        Matrix A(n, n);
        for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) A[i][j] = R(0, 2) ? R(-9, 9) : 0;
        assert(A.det() == brute_det(A.arr));
    }

    Lagrange L;
    L.construct_inv_fac();
    for (int it = 0; it < 300; it++) {
        int d = R(0, 8), X0 = R(-20, 20);
        vector<int> c(d + 1);
        for (auto &x : c) x = R(0, mod - 1);
        auto f = [&](int x) {
            x = (x % mod + mod) % mod;
            int r = 0;
            for (int i = d; i >= 0; i--) r = (r * x + c[i]) % mod;
            return r;
        };
        vector<int> v(d + 1);
        for (int i = 0; i <= d; i++) v[i] = f(X0 + i);
        L.init(X0, v);
        for (int x : {X0 - 1, X0 - 2, X0 - 30, X0 + d + 5, (int)mod - 1, -(int)mod + 3, (int)1e18})
            assert(L.sample(x) == f(x));
    }

    for (int n = 0; n <= 300; n++) {
        auto ph = phi_1_to_n(n);
        assert((int)ph.size() == n + 1 && ph[0] == 0);
        for (int i = 1; i <= n; i++) assert(ph[i] == phi(i));
    }

    for (int k = 0; k <= 6; k++) {
        vector<int> a(1 << k), b(1 << k), c(1 << k);
        for (auto &x : a) x = R(-100, 100);
        for (auto &x : b) x = R(-100, 100);
        for (int i = 0; i < (1 << k); i++) for (int j = 0; j < (1 << k); j++) c[i ^ j] += a[i] * b[j];
        assert(xor_convolution(a, b, k) == c);
    }
    return 0;
}
