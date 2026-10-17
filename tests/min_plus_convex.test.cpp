// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/min_plus_convolution_convex_convex
#include "common.hpp"
// 檔案用到的巨集（codebook 沒附），這裡自己補；ssize 是 C++20，補成 C++17 版本
#define FOR(i, a, b) for (int i = (a); i <= (b); i++)
#define pb push_back
#define all(x) (x).begin(), (x).end()
template <class T> int ssize(const T &v) { return v.size(); }
#include "../codes/Convolution/Min_Convolution_Concave_Concave.cpp"

signed main() {
    ios::sync_with_stdio(0), cin.tie(0);
    int n, m;
    cin >> n >> m;
    vector<int> a(n), b(m);
    for (auto &x : a) cin >> x;
    for (auto &x : b) cin >> x;
    for (int x : mkk(a, b)) cout << x << ' ';
    cout << '\n';
}
