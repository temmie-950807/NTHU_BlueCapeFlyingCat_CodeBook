// competitive-verifier: STANDALONE
#include "common.hpp"
#include <cassert> // GCC 16 的 bits/stdc++.h 不含 assert
const int MAX_N = 105;
#include "../codes/Data-Structure/Treap2.cpp"

// 區間加值、區間和、get_pos 與暴力比對；每輪 nodeCnt = 0 重用節點
signed main() {
    srand(7122);
    mt19937_64 rng(7122);
    auto rnd = [&](int l, int r) { return (int)(rng() % (r - l + 1)) + l; };
    for (int tc = 0; tc < 2000; tc++) {
        nodeCnt = root = 0;
        int n = rnd(1, 100);
        vector<int> a(n), id(n);
        for (int i = 0; i < n; i++) a[i] = rnd(-9, 9), id[i] = new_node(a[i]), root = merge(root, id[i]);
        for (int op = 0; op < 30; op++) {
            int l = rnd(1, n), r = rnd(1, n);
            if (l > r) swap(l, r);
            int v = rnd(-5, 5);
            auto [x, y, z] = cut(root, l, r);
            int sum = 0;
            for (int i = l - 1; i < r; i++) sum += a[i], a[i] += v;
            assert(sz[y] == r - l + 1 && total[y] == sum);
            apply(y, v);
            root = merge(merge(x, y), z);
            int p = rnd(0, n - 1);
            assert(get_pos(id[p]) == p + 1);
        }
    }
    puts("OK");
}
