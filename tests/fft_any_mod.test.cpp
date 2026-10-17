// competitive-verifier: STANDALONE
#include "common.hpp"
int qp(int b, int p, int m) { // NTT 需要的快速冪
    int r = 1;
    for (b %= m; p; p >>= 1, b = b * b % m) if (p & 1) r = r * b % m;
    return r;
}
namespace ntt {
#include "../codes/Convolution/NTT_mod_998244353.cpp"
}
#include "../codes/Convolution/FFT_any_mod.cpp"

int cut2m1() { int c = sqrt(MOD); return c * c - 1; } // 高低位都是 cut-1
// FFT_any_mod（MOD = 998244353）與暴力 / NTT 比對，含精度最差的全 MOD-1 輸入
signed main() {
    mt19937_64 rng(7122);
    auto naive = [&](const vector<int> &a, const vector<int> &b) {
        vector<int> c(a.size() + b.size() - 1);
        for (size_t i = 0; i < a.size(); i++)
            for (size_t j = 0; j < b.size(); j++) c[i + j] = (c[i + j] + a[i] * b[j]) % MOD;
        return c;
    };
    assert(PolyMul({}, {1, 2}).empty() && PolyMul({3}, {}).empty());
    for (int it = 0; it < 300; it++) {
        int n = rng() % 70 + 1, m = rng() % 70 + 1;
        if (it < 20) n = 1 << (it % 7), m = (1 << (it % 5)) + 1 - n % 2; // n+m-1 剛好是 2 的冪附近
        vector<int> a(n), b(m);
        for (auto &x : a) x = it % 3 ? rng() % MOD : MOD - 1 - rng() % 2;
        for (auto &x : b) x = it % 3 ? rng() % MOD : MOD - 1 - rng() % 2;
        assert(PolyMul(a, b) == naive(a, b));
    }
    // 全部同值（精度最差）：答案是 v^2 * 項數。2^19 的最壞情況誤差已逼近 0.5，這裡只測 2^17
    for (int v : {MOD - 1, cut2m1()}) {
        int N = 1 << 17, M = N - 1;
        vector<int> a(N, v), b(M, v), c = PolyMul(a, b);
        assert((int)c.size() == N + M - 1);
        for (int k = 0; k < N + M - 1; k++) assert(c[k] == v * v % MOD * min({k + 1, N + M - 1 - k, M}) % MOD);
    }
    int N = 1 << 19;
    vector<int> a(N), b(N);
    // 隨機大值與 NTT 比對
    for (auto &x : a) x = rng() % MOD;
    for (auto &x : b) x = rng() % MOD;
    assert(PolyMul(a, b) == ntt::polyMul(a, b));
    puts("OK");
    return 0;
}
