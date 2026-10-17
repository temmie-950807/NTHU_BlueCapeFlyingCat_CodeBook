// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/point_add_range_sum
#include "common.hpp"
#include "../codes/Data-Structure/BIT.cpp"

signed main() {
    ios::sync_with_stdio(0), cin.tie(0);
    int n, q;
    cin >> n >> q;
    for (int i = 1, x; i <= n; i++) cin >> x, add(i, x);
    while (q--) {
        int t, a, b;
        cin >> t >> a >> b;
        if (t == 0) add(a + 1, b);
        else cout << query(b) - query(a) << '\n';
    }
}
