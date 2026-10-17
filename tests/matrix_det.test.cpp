// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/matrix_det
#include "common.hpp"
const int MOD = 998244353;
int qp(int b, int p, int m) {
    int r = 1;
    for (b %= m; p; p >>= 1, b = b * b % m) if (p & 1) r = r * b % m;
    return r;
}
#include "../codes/Math/Matrix.cpp"

signed main() {
    ios::sync_with_stdio(0), cin.tie(0);
    int n;
    cin >> n;
    Matrix a(n, n);
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) cin >> a[i][j];
    cout << a.det() << '\n';
}
