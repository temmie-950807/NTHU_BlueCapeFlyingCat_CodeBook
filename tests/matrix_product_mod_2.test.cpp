// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/matrix_product_mod_2
#include "common.hpp"
#include "../codes/Math/Matrix_01.cpp"

signed main() {
    ios::sync_with_stdio(0), cin.tie(0);
    int n, m, k;
    cin >> n >> m >> k;
    Matrix a(n, m), b(m, k);
    string s;
    for (int i = 0; i < n; i++) {
        cin >> s;
        for (int j = 0; j < m; j++) a.arr[i][j] = s[j] - '0';
    }
    for (int i = 0; i < m; i++) {
        cin >> s;
        for (int j = 0; j < k; j++) b.arr[i][j] = s[j] - '0';
    }
    Matrix c = a * b;
    for (int i = 0; i < n; i++) {
        s.assign(k, '0');
        for (int j = 0; j < k; j++) s[j] += c.arr[i][j];
        cout << s << '\n';
    }
}
