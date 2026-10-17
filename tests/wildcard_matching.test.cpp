// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/wildcard_pattern_matching
#include "common.hpp"
#include "../codes/Convolution/FFT.cpp"

signed main() {
    ios::sync_with_stdio(0), cin.tie(0);
    string s, t;
    cin >> s >> t;
    #include "../codes/String/Wildcard_Matching.cpp"
    cout << '\n';
}
