// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/assignment
#include "common.hpp"
#include "../codes/Graph/Kuhn_Munkres.cpp"

signed main() {
    ios::sync_with_stdio(0), cin.tie(0);
    int n;
    cin >> n;
    KuhnMunkres km(n);
    // 完全二分圖、可負權：直接寫 G（add 會被預設的 0 擋掉負權）
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) cin >> km.G[i][j], km.G[i][j] *= -1;
    int ans = -km.solve();
    vector<int> p(n);
    for (int j = 0; j < n; j++) p[km.match[j]] = j;
    cout << ans << '\n';
    for (int i = 0; i < n; i++) cout << p[i] << " \n"[i == n - 1];
}
