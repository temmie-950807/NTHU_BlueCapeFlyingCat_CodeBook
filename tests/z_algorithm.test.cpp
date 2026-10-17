// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/zalgorithm
#include "common.hpp"
#include "../codes/String/Z_Algorithm.cpp"

signed main() {
    string s;
    cin >> s;
    vector<int> z = z_function(s);
    z[0] = s.size(); // codebook 定義 z[0] = 0，題目要 |s|
    for (int x : z) cout << x << ' ';
    cout << '\n';
}
