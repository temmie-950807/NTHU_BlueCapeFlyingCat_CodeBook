// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/convolution_mod
#include "common.hpp"
int qp(int b, int p, int m) { // NTT 需要的快速冪，比賽時自己補
    int r = 1;
    for (b %= m; p; p >>= 1, b = b * b % m) if (p & 1) r = r * b % m;
    return r;
}
#include "../codes/Convolution/NTT_mod_998244353.cpp"

signed main() {
    ios::sync_with_stdio(0), cin.tie(0);
    int n, m;
    cin >> n >> m;
    vector<int> a(n), b(m);
    for (auto &x : a) cin >> x;
    for (auto &x : b) cin >> x;
    for (int x : polyMul(a, b)) cout << x << ' ';
    cout << '\n';
}
