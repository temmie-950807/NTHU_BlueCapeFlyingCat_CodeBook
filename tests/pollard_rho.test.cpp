// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/factorize
#include "common.hpp"
#include "../codes/Math/Miller_Rabin.cpp"
#include "../codes/Math/Pollard_Rho.cpp"

void factor(int n, vector<int> &out) {
    if (n == 1) return;
    if (is_prime(n)) return out.push_back(n);
    if (n % 2 == 0) return out.push_back(2), factor(n / 2, out);
    int d = n;
    while (d == n) d = Pollard_Rho(n); // Pollard_Rho 可能回傳 n 本身，要重試
    factor(d, out), factor(n / d, out);
}

signed main() {
    ios::sync_with_stdio(0), cin.tie(0);
    int q;
    cin >> q;
    while (q--) {
        int a;
        cin >> a;
        vector<int> f;
        factor(a, f);
        sort(f.begin(), f.end());
        cout << f.size();
        for (int x : f) cout << ' ' << x;
        cout << '\n';
    }
}
