// competitive-verifier: STANDALONE
#include "common.hpp"
#define main digit_dp_main // 檔案自帶 main，改名後直接呼叫 find_answer
#include "../codes/Dynamic-Programming/Digit_DP.cpp"
#undef main

bool good(long long x) {
    string s = to_string(x);
    for (int i = 0; i + 1 < (int)s.size(); i++) if (s[i] == s[i + 1]) return false;
    return true;
}

signed main() {
    // [0, n] 暴力
    const int MX = 20000;
    vector<long long> pre(MX + 1);
    for (int x = 0; x <= MX; x++) pre[x] = (x ? pre[x - 1] : 0) + good(x);
    for (int n = 0; n <= MX; n++) assert(find_answer(n) == pre[n]);
    assert(find_answer(-1) == 0); // l = 0 時會呼叫 find_answer(-1)
    // l = r、l = 0 區間
    mt19937_64 rng(7122);
    for (int it = 0; it < 2000; it++) {
        long long l = rng() % (MX + 1), r = rng() % (MX + 1);
        if (l > r) swap(l, r);
        if (it % 3 == 0) r = l;
        if (it % 5 == 0) l = 0;
        assert(find_answer(r) - find_answer(l - 1) == pre[r] - (l ? pre[l - 1] : 0));
    }
    // 大數：[0, 10^k - 1] 有 1 + 9 + 9^2 + ... + 9^k 個
    long long p10 = 1, sum = 1, p9 = 1;
    for (int k = 1; k <= 18; k++) {
        p10 *= 10, p9 *= 9, sum += p9;
        assert(find_answer(p10 - 1) == sum);
        assert(find_answer(p10) == sum + (k == 1)); // 10^k (k >= 2) 本身有連續的 0
    }
    // 大數區間：1e18、LLONG_MAX 附近暴力（base + 2000 不能溢位）
    for (long long base : {(long long)1e18 - 5000, (long long)9e18 - 3000, LLONG_MAX - 2001}) {
        long long cnt = 0;
        for (long long x = base; x <= base + 2000; x++) {
            cnt += good(x);
            assert(find_answer(x) - find_answer(base - 1) == cnt);
        }
    }
    return 0;
}
