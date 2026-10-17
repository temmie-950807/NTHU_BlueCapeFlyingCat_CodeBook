// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/convolution_mod
// 檔案的 MOD 寫死 998244353（1e9+7 版本要改原始碼，無法在測試裡切換）
#include "common.hpp"
#include "../codes/Convolution/FFT_any_mod.cpp"

signed main() {
    ios::sync_with_stdio(0), cin.tie(0);
    int n, m;
    cin >> n >> m;
    vector<int> a(n), b(m);
    for (auto &x : a) cin >> x;
    for (auto &x : b) cin >> x;
    for (int x : PolyMul(a, b)) cout << x << ' ';
    cout << '\n';
}
