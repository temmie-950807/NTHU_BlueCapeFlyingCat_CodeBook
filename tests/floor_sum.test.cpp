// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/sum_of_floor_of_linear
#include "common.hpp"
#include "../codes/Math/Floor_Sum.cpp"

signed main() {
    ios::sync_with_stdio(0), cin.tie(0);
    int t;
    cin >> t;
    while (t--) {
        int n, m, a, b;
        cin >> n >> m >> a >> b;
        cout << floor_sum(n, m, a, b) << '\n';
    }
}
