// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/suffixarray
#include "common.hpp"
#include "../codes/Data-Structure/Sparse_Table.cpp"
#include "../codes/String/Suffix_Array.cpp"

signed main() {
    ios::sync_with_stdio(0), cin.tie(0);
    string s;
    cin >> s;
    SuffixArray sa(s);
    for (int x : sa.sa) cout << x << ' ';
    cout << '\n';
}
