// competitive-verifier: STANDALONE
#include <cassert>
#include "common.hpp"
const int MOD = 998244353;
// codebook 的 qp 在 Math/Miller_Rabin.cpp；這裡用只吃 [0, m) 的版本，確保 d 先被調成非負
int qp(int b, int p, int m) {
    assert(0 <= b && b < m);
    int r = 1;
    for (; p; p >>= 1, b = b * b % m) if (p & 1) r = r * b % m;
    return r;
}
#include "../codes/Misc/OEIS.cpp"

signed main() {
    mt19937_64 rng(7122);
    assert((BerlekampMassey({1, 1, 3, 7, 17}) == vector<int>{1, 2})); // 註解的範例
    assert((BerlekampMassey({1, -1, 1, -1}) == vector<int>{MOD - 1})); // 輸入有負數：s[i] = -s[i-1]
    for (int it = 0; it < 2000; it++) {
        int k = rng() % 12 + 1;
        vector<int> c(k), s(2 * k + rng() % 5); // c[j] 乘 s[i-k+j]，跟 BerlekampMassey 回傳順序相同
        for (int &x : c) x = rng() % MOD;
        if (it % 2) for (int &x : c) x = MOD - 1 - rng() % 3; // 係數 -1, -2, -3
        if (!c[0]) c[0] = 1;
        for (int i = 0; i < (int)s.size(); i++) {
            if (i < k) { s[i] = rng() % MOD; continue; }
            for (int j = 0; j < k; j++) s[i] = (s[i] + c[j] * s[i - k + j]) % MOD;
        }
        vector<int> r = BerlekampMassey(s);
        for (int x : r) assert(0 <= x && x < MOD);
        if (r == c) continue;
        // 極少數情況初始值剛好讓最小遞迴變短：只要求回傳的遞迴能生成整個序列且不比 k 長
        assert((int)r.size() < k);
        for (int i = r.size(); i < (int)s.size(); i++) {
            int v = 0;
            for (int j = 0; j < (int)r.size(); j++) v = (v + r[j] * s[i - r.size() + j]) % MOD;
            assert(v == s[i]);
        }
    }
    return 0;
}
