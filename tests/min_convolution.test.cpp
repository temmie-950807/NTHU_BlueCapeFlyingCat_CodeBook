// competitive-verifier: STANDALONE
#include "common.hpp"
// 檔案用到的巨集（codebook 沒附），這裡自己補；ssize 是 C++20，補成 C++17 版本
#define FOR(i, a, b) for (int i = (a); i <= (b); i++)
#define pb push_back
#define all(x) (x).begin(), (x).end()
template <class T> int ssize(const T &v) { return v.size(); }
#include "../codes/Convolution/Min_Convolution_Concave_Concave.cpp"

// 只對凸序列（差分遞增）成立，與暴力 min-plus 比對
signed main() {
    mt19937_64 rng(7122);
    auto convex = [&](int n, int lim) {
        vector<int> d(n);
        for (auto &x : d) x = (int)(rng() % (2 * lim + 1)) - lim;
        sort(d.begin(), d.end());
        vector<int> a(n);
        a[0] = (int)(rng() % (2 * lim + 1)) - lim;
        for (int i = 1; i < n; i++) a[i] = a[i - 1] + d[i];
        return a;
    };
    for (int it = 0; it < 2000; it++) {
        int n = rng() % 40 + 1, m = rng() % 40 + 1, lim = it % 2 ? 3 : 1e9;
        if (it < 4) n = 1 + it / 2, m = 1 + it % 2;
        vector<int> a = convex(n, lim), b = convex(m, lim), c(n + m - 1, INF);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) c[i + j] = min(c[i + j], a[i] + b[j]);
        assert(mkk(a, b) == c);
    }
    puts("OK");
    return 0;
}
