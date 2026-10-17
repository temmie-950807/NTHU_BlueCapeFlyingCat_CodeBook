// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/sort_points_by_argument
// Geometry 06 point::operator<（極角排序）。它的範圍是 (0°, 360°]，且不接受零向量，
// 所以這裡先拿掉零向量排序，再轉成題目要的 (-180°, 180°]（零向量的角度算 0）
#include "common.hpp"
#define pb push_back
mt19937 rngf(7122);
#include "geometry_lib.hpp"

signed main() {
    ios::sync_with_stdio(false), cin.tie(nullptr);
    int n;
    cin >> n;
    vector<point<int>> v, zero;
    for (int i = 0; i < n; i++) {
        point<int> p;
        cin >> p.x >> p.y;
        (p.x == 0 && p.y == 0 ? zero : v).push_back(p);
    }
    sort(v.begin(), v.end());
    // v = [(0°, 180°]] [(180°, 360°)] [360°]
    vector<point<int>> up, low, pos;
    for (auto &p : v) (p.y > 0 || (p.y == 0 && p.x < 0) ? up : p.y < 0 ? low : pos).push_back(p);
    for (auto *g : {&low, &zero, &pos, &up})
        for (auto &p : *g) cout << p.x << ' ' << p.y << '\n';
}
