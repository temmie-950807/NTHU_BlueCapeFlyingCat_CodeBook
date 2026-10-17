// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/enumerate_quotients
#include "common.hpp"

signed main() {
    int n;
    cin >> n;
    vector<int> ans; // 每個區塊的右端點 r
#include "../codes/Math/數論分塊.cpp"
    cout << ans.size() << '\n';
    for (int i = ans.size() - 1; i >= 0; i--) cout << n / ans[i] << " \n"[i == 0];
}
