// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/inv_of_formal_power_series
#include "common.hpp"
int qp(int b, int p, int m) { // NTT 需要的快速冪，比賽時自己補
    int r = 1;
    for (b %= m; p; p >>= 1, b = b * b % m) if (p & 1) r = r * b % m;
    return r;
}
#include "../codes/Convolution/NTT_mod_998244353.cpp"
#include "../codes/Convolution/PolyInv.cpp"

signed main() {
    ios::sync_with_stdio(0), cin.tie(0);
    int n;
    cin >> n;
    vector<int> a(n);
    for (auto &x : a) cin >> x;
    for (int x : polyInv(a)) cout << x << ' ';
    cout << '\n';
}
