// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/primality_test
#include "common.hpp"
#include "../codes/Math/Miller_Rabin.cpp"

signed main() {
    ios::sync_with_stdio(0), cin.tie(0);
    int q;
    cin >> q;
    while (q--) {
        int n;
        cin >> n;
        cout << (is_prime(n) ? "Yes" : "No") << '\n';
    }
}
