// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/bitwise_and_convolution
#include "common.hpp"

// dp[S] = sum_{T subset S} dp[T]
void sos(vector<int> &dp, int n) {
#include "../codes/Dynamic-Programming/SOS_DP.cpp"
}

const int MOD = 998244353;

// AND 卷積：取補集變成子集和；反演 = 正負號翻轉 + 子集和 + 再翻轉
signed main() {
    ios::sync_with_stdio(0), cin.tie(0);
    int n;
    cin >> n;
    int full = (1 << n) - 1;
    vector<int> a(1 << n), b(1 << n), A(1 << n), B(1 << n), C(1 << n);
    for (auto &x : a) cin >> x;
    for (auto &x : b) cin >> x;
    for (int s = 0; s <= full; s++) A[s] = a[full ^ s], B[s] = b[full ^ s];
    sos(A, n), sos(B, n);
    for (int s = 0; s <= full; s++) {
        C[s] = A[s] % MOD * (B[s] % MOD) % MOD;
        if (__builtin_popcountll(s) & 1) C[s] = (MOD - C[s]) % MOD;
    }
    sos(C, n);
    for (int s = 0; s <= full; s++) {
        C[s] %= MOD;
        if (__builtin_popcountll(s) & 1) C[s] = (MOD - C[s]) % MOD;
    }
    for (int k = 0; k <= full; k++) cout << C[full ^ k] << " \n"[k == full];
}
