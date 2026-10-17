// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/runenumerate
#include "common.hpp"
#include "../codes/Data-Structure/Sparse_Table.cpp"
#include "../codes/String/Suffix_Array.cpp"
#include "../codes/String/Enumerate_Runs.cpp"

signed main() {
    ios::sync_with_stdio(0), cin.tie(0);
    string s;
    cin >> s;
    auto runs = enumerate_run(s);
    cout << runs.size() << '\n';
    for (auto [t, l, r] : runs) cout << t << ' ' << l << ' ' << r << '\n';
}
