// competitive-verifier: STANDALONE
// CRT（不互質、無解、負餘數、lcm 近 1e18）、兩份 Josephus、Lagrange_any_x、Linear_Mod_Inverse、Lucas、數論分塊、Matrix_01 對暴力
#include "common.hpp"
const int MOD = 998244353;
int qp(int b, int p, int m) {
    int r = 1;
    for (b %= m; p; p >>= 1, b = b * b % m) if (p & 1) r = r * b % m;
    return r;
}
int C(int n, int k, int p) { // 給 Lucas 用，n < p，k > n 回傳 0
    if (k < 0 || k > n) return 0;
    int a = 1, b = 1;
    for (int i = 0; i < k; i++) a = a * ((n - i) % p) % p, b = b * ((i + 1) % p) % p;
    return a * qp(b, p - 2, p) % p;
}
#include "../codes/Math/CRT.cpp"
#include "../codes/Math/Josephus_Problem.cpp"
#include "../codes/Math/josephus.cpp"
#include "../codes/Math/Lagrange_any_x.cpp"
#include "../codes/Math/Lucas's_Theorem.cpp"
#include "../codes/Math/Matrix_01.cpp"

mt19937_64 rng(7122);
int R(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }

vector<int> lin_inv(vector<int> a, int p) {
    int n = a.size() - 1;
#include "../codes/Math/Linear_Mod_Inverse.cpp"
    return invA;
}
vector<int> blocks(int n) {
    vector<int> ans;
#include "../codes/Math/數論分塊.cpp"
    return ans;
}

void test_crt() {
    for (int it = 0; it < 30000; it++) { // 小模數暴力，至少兩條式子
        int k = R(2, 4);
        vector<ll> a(k), m(k);
        for (int i = 0; i < k; i++) m[i] = R(1, 12), a[i] = R(-30, 30);
        int L = 1;
        for (int i = 0; i < k; i++) L = lcm(L, (int)m[i]);
        int sol = -1;
        for (int x = 0; x < L && sol == -1; x++) {
            bool ok = 1;
            for (int i = 0; i < k; i++) ok &= (x - (int)a[i]) % (int)m[i] == 0;
            if (ok) sol = x;
        }
        auto r = CRT_arr(a, m);
        if (sol == -1) assert(r.first == -1);
        else assert(r.first == sol && r.second == L);
    }
    for (int it = 0; it < 30000; it++) { // lcm 接近 1e18
        ll g = R(1, 1000), m1 = g * R(1, 1e9), m2 = g * R(1, 1e9);
        ll L = m1 / __gcd(m1, m2) * m2, x = (ll)R(0, 1e18) % L;
        auto r = CRT(x % m1 - m1 * R(0, 1), m1, x % m2 + m2 * R(0, 3), m2);
        assert(r.first == x && r.second == L);
    }
}

void test_josephus() {
    for (int n = 1; n <= 200; n++) { // 每數到偶數刪一個，第 k 個被刪的人（1-based）
        vector<int> v(n);
        iota(v.begin(), v.end(), 1);
        int idx = 0;
        for (int k = 1; k <= n; k++) {
            idx = (idx + 1) % v.size();
            assert(solve(n, k) == v[idx]);
            v.erase(v.begin() + idx);
            if (!v.empty()) idx %= v.size();
        }
    }
    for (int n = 1; n <= 200; n++)
        for (int k = 1; k <= 250; k++) { // 存活者（0-based），含 k = 1、k > n
            int r = 0;
            for (int i = 2; i <= n; i++) r = (r + k) % i;
            assert(josephus(n, k) == r);
        }
    for (int k : {2, 3, 1000}) {
        int n = 3000000, r = 0;
        for (int i = 2; i <= n; i++) r = (r + k) % i;
        assert(josephus(n, k) == r);
    }
    int big = 1e18;
    assert(solve(big, big) == josephus(big, 2) + 1); // 最後被刪的就是存活者
}

void test_lagrange() {
    for (int it = 0; it < 2000; it++) {
        int n = R(1, 8);
        vector<int> c(n);
        for (auto &x : c) x = R(0, MOD - 1);
        auto f = [&](int x) {
            x %= MOD;
            int r = 0;
            for (int i = n - 1; i >= 0; i--) r = (r * x + c[i]) % MOD;
            return r;
        };
        set<int> xs;
        while ((int)xs.size() < n) xs.insert(it % 2 ? R(0, 20) : R(0, MOD - 1));
        vector<pair<int, int>> v;
        for (int x : xs) v.push_back({x, f(x)});
        Lagrange L(v);
        for (int x : {v[R(0, n - 1)].first, R(0, MOD - 1), R(0, 1e18)}) assert(L.solve(x) == f(x));
    }
}

void test_inv() {
    for (int p : {2, 3, 5, 13, 1000003, 998244353, 1000000007}) {
        int n = min(p - 1, 100000LL);
        vector<int> a(n + 1);
        for (int i = 1; i <= n; i++) a[i] = p <= 100001 ? i : R(1, p - 1);
        auto inv = lin_inv(a, p);
        for (int i = 1; i <= n; i++) assert(a[i] * inv[i] % p == 1);
    }
}

void test_lucas() {
    for (int p : {2, 3, 5, 7, 13}) {
        const int N = 200;
        vector<vector<int>> P(N + 1, vector<int>(N + 1, 0));
        for (int i = 0; i <= N; i++) {
            P[i][0] = 1;
            for (int j = 1; j <= i; j++) P[i][j] = (P[i - 1][j - 1] + P[i - 1][j]) % p;
        }
        for (int n = 0; n <= N; n++)
            for (int k = 0; k <= N + 5; k++) assert(Lucas(n, k, p) == (k <= n ? P[n][k] : 0));
    }
    for (int it = 0; it < 10000; it++) { // n 很大：Kummer，沒有進位才非 0
        int p = vector<int>{2, 3, 5, 7}[R(0, 3)], n = R(0, 1e18), k = R(0, n), a = n, b = k;
        bool nz = 1;
        for (; b; a /= p, b /= p) nz &= b % p <= a % p;
        assert((Lucas(n, k, p) != 0) == nz);
    }
}

void test_blocks() {
    for (int n = 1; n <= 2000; n++) {
        set<int> q;
        for (int i = 1; i <= n; i++) q.insert(n / i);
        auto b = blocks(n);
        assert(b.size() == q.size() && b.back() == n);
        int prev = 0;
        for (int r : b) {
            assert(r > prev && n / (prev + 1) == n / r && q.count(n / r));
            prev = r;
        }
    }
}

void test_matrix01() {
    for (int it = 0; it < 200; it++) {
        int n = R(1, 40), m = R(1, 40), k = R(1, 40);
        Matrix A(n, m), B(m, k);
        vector<vector<int>> a(n, vector<int>(m)), b(m, vector<int>(k));
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) A.arr[i][j] = a[i][j] = R(0, 1);
        for (int i = 0; i < m; i++)
            for (int j = 0; j < k; j++) B.arr[i][j] = b[i][j] = R(0, 1);
        Matrix Cm = A * B;
        assert(Cm.n == n && Cm.m == k);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < k; j++) {
                int s = 0;
                for (int t = 0; t < m; t++) s ^= a[i][t] & b[t][j];
                assert(Cm.arr[i][j] == s);
            }
    }
}

signed main() {
    test_crt();
    test_josephus();
    test_lagrange();
    test_inv();
    test_lucas();
    test_blocks();
    test_matrix01();
    return 0;
}
