// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/enumerate_palindromes
#include "common.hpp"
#include "../codes/String/Manacher.cpp"

signed main() {
    ios::sync_with_stdio(0), cin.tie(0);
    string s;
    cin >> s;
    // 每個中心 c = l+r 的最長迴文長度；沒有出現在結果中的中心長度為 0
    vector<int> len(2 * s.size() - 1, 0);
    for (auto [l, r] : Manacher(s)) len[l + r] = r - l + 1;
    for (int x : len) cout << x << ' ';
    cout << '\n';
}
